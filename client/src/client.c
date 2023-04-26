#include "../inc/client.h"

int main(int argc, char** argv) {

	if(argc != 3) {
        mx_printerr("usage: ./uchar [ip] [port]\n");
        exit(-1);
    }

    client_t client;
    mx_client_init(&client, argv[1], atoi(argv[2]));

	gtk_init(&argc, &argv);

	client.a_window = mx_build_auth_window(&client);
	client.c_window = mx_build_chat_window(&client);
	client.create_chat_window = mx_build_create_chat_window(&client);
	client.d_window = mx_build_dialog_window(&client);
	client.i_window = mx_build_chat_info_window(&client);
	client.u_window = mx_build_user_info_window(&client);

	client.current_response = NULL;
	client.search_mode = false;
	client.edit_mode = false;
	client.current_chat_id = -1;
	client.last_msg_in_chat_id = -1;
	client.first_msg_in_chat_id = -1;

	//client.chat_id_chat_btn_map = NULL;
	client.messages_in_current_chat = NULL;
	client.p_change_clear_list = NULL;
	client.request_queue = mx_create_queue();
	client.message_id_row_map = mx_create_map();

	srand(time(NULL));
	

	pthread_t thread;
	pthread_create(&thread, NULL, (void *(*)(void *))&mx_main_background_loop, (void*)&client);

	//create_new_chat_window(&client);

	gtk_widget_show(client.a_window->window);

	gtk_main();
	pthread_join(thread, NULL);
	return 0;
}

