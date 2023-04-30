#include "../inc/client.h"

static inline char* mx_permission_to_str(int perm){
	switch(perm){
		case TYPE_USER: return "User:";
		case TYPE_ADMIN: return "Admin:";
		case TYPE_OWNER: return "Owner:";
		default: return "User";
	}
}


void mx_on_ban_btn_clicked(GtkButton* b, gpointer data){
	client_t* client = (client_t*)data;
	perm_change_t* retrieved_p_change = (perm_change_t*)g_object_get_data(G_OBJECT(b), "p_change");
	int user_id = retrieved_p_change->userID;

	if(retrieved_p_change->this_client_permissions <= retrieved_p_change->user_permissions)
		return;

	request_t* request = (request_t*)malloc(sizeof(request_t));
	request->req = mx_create_ban_req(client->current_chat_id, user_id, 1);


	gtk_button_set_label(GTK_BUTTON(b), "unban");
	g_signal_connect(G_OBJECT(b), "released", G_CALLBACK(mx_on_unban_btn_clicked), client);


	mx_queue_push(client->request_queue, request);
}

void mx_on_unban_btn_clicked(GtkButton* b, gpointer data){
	client_t* client = (client_t*)data;
	perm_change_t* retrieved_p_change = (perm_change_t*)g_object_get_data(G_OBJECT(b), "p_change");
	int user_id = retrieved_p_change->userID;

	if(retrieved_p_change->this_client_permissions <= retrieved_p_change->user_permissions)
		return;

	request_t* request = (request_t*)malloc(sizeof(request_t));
	request->req = mx_create_ban_req(client->current_chat_id, user_id, 0);


	gtk_button_set_label(GTK_BUTTON(b), "ban");
	g_signal_connect(G_OBJECT(b), "released", G_CALLBACK(mx_on_ban_btn_clicked), client);


	mx_queue_push(client->request_queue, request);
}

void mx_on_promote_btn_cliked(GtkButton* b, gpointer data){
	client_t* client = (client_t*)data;
	perm_change_t* p_change = (perm_change_t*)g_object_get_data(G_OBJECT(b), "p_change");
	int user_id = p_change->userID;

	if(p_change->this_client_permissions > p_change->user_permissions){
		p_change->user_permissions += 1;
		gtk_label_set_text(GTK_LABEL(p_change->p_label), mx_permission_to_str(p_change->user_permissions));

		request_t* request = (request_t*)malloc(sizeof(request_t));
		request->req = mx_create_permissions_change_req(client->current_chat_id, user_id, p_change->user_permissions);
		mx_queue_push(client->request_queue, request);
	}
}

void mx_on_demote_btn_clicked(GtkButton* b, gpointer data){
	client_t* client = (client_t*)data;
	perm_change_t* p_change = (perm_change_t*)g_object_get_data(G_OBJECT(b), "p_change");
	int user_id = p_change->userID;

	if(p_change->user_permissions - 1 < TYPE_USER)
		return;

	if(p_change->this_client_permissions > p_change->user_permissions){
		p_change->user_permissions -= 1;
		gtk_label_set_text(GTK_LABEL(p_change->p_label), mx_permission_to_str(p_change->user_permissions));

		request_t* request = (request_t*)malloc(sizeof(request_t));
		request->req = mx_create_permissions_change_req(client->current_chat_id, user_id, p_change->user_permissions);
		mx_queue_push(client->request_queue, request);
	}
}


static inline gboolean push_uifo_in_gtk_loop(gpointer data){
	members_list_entry_t* m = (members_list_entry_t*)data;

	GtkWidget* user_main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget* user_left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(user_main_box), user_left_box, TRUE, TRUE, 0);
	
	GtkWidget* username_label = gtk_label_new(m->user_to_add);
	gtk_widget_set_name (username_label, "username_label");
	mx_widget_add_styles(username_label);

	GtkWidget* user_per = gtk_label_new(mx_permission_to_str(m->member_permissions));
	gtk_widget_set_name (user_per, "user_per_label");
	mx_widget_add_styles(user_per);

	gtk_box_pack_start(GTK_BOX(user_left_box), user_per, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(user_left_box), username_label, TRUE, TRUE, 0);


	GtkWidget* user_button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(user_main_box), user_button_box, FALSE, FALSE, 0);

	if(m->this_client_permissions > m->member_permissions){

		perm_change_t* p_change = (perm_change_t*)malloc(sizeof(perm_change_t));
		p_change->p_label = user_per;
		p_change->this_client_permissions = m->this_client_permissions;
		p_change->user_permissions = m->member_permissions;
		p_change->userID = m->userID_to_add;

		mx_push_back(&(m->client->p_change_clear_list), p_change);

		// ban unban
		//promote demote
		bool ban_f = m->banned == 1;

		GtkWidget* ban_btn = gtk_button_new_with_label((ban_f ? "unban" : "ban"));

		gtk_widget_set_name (ban_btn, "ban_btn");
		mx_widget_add_styles(ban_btn);

		gtk_box_pack_end(GTK_BOX(user_button_box), ban_btn, FALSE, FALSE, 0);
		g_object_set_data(G_OBJECT(ban_btn), "p_change", p_change);
		g_signal_connect(G_OBJECT(ban_btn), "released", G_CALLBACK(ban_f ? mx_on_unban_btn_clicked : mx_on_ban_btn_clicked), m->client);

	
	    GtkWidget* promote_btn = gtk_button_new_with_label("promote");
		gtk_box_pack_end(GTK_BOX(user_button_box), promote_btn, FALSE, FALSE, 0);

		gtk_widget_set_name (promote_btn, "promote_btn");
		mx_widget_add_styles(promote_btn);

		g_object_set_data(G_OBJECT(promote_btn), "p_change", p_change);
		g_signal_connect(G_OBJECT(promote_btn), "released", G_CALLBACK(mx_on_promote_btn_cliked), m->client);

		
		GtkWidget* demote_btn = gtk_button_new_with_label("demote");
		gtk_box_pack_end(GTK_BOX(user_button_box), demote_btn, FALSE, FALSE, 0);

		gtk_widget_set_name (demote_btn, "demote_btn");
		mx_widget_add_styles(demote_btn);

		g_object_set_data(G_OBJECT(demote_btn), "p_change", p_change);
		g_signal_connect(G_OBJECT(demote_btn), "released", G_CALLBACK(mx_on_demote_btn_clicked), m->client);
		
	}
	
	//GtkListBoxRow* row = gtk_list_box_row_new();
	GtkListBoxRow* row = (GtkListBoxRow*)gtk_list_box_row_new();
	gtk_widget_set_name(GTK_WIDGET(row), "user_row");
	mx_widget_add_styles(GTK_WIDGET(row));
	
	//gtk_container_add(GTK_CONTAINER(row), user_main_box);
    gtk_container_add(GTK_CONTAINER(GTK_WIDGET(row)), user_main_box);
    
	//gtk_list_box_insert(GTK_LIST_BOX(m->client->i_window->members_list_box), row, -1);
	gtk_list_box_insert(GTK_LIST_BOX(m->client->i_window->members_list_box), GTK_WIDGET(row), -1);

	gtk_widget_show_all(m->client->i_window->members_list_box);


	free(m->user_to_add);
	free(m);
	return FALSE;
}

void mx_user_info_add_user(members_list_entry_t* m){
	g_idle_add(push_uifo_in_gtk_loop, m);
}
