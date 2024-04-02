#include <chat_client/Application.hpp>

int main(int argc, char** argv) {
	chat_client::Application application(argc, argv);

	application.start_application();

	return 0;
}

