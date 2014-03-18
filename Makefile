CFLAGS = -g -O0  -Wall -Werror
LDFLAGS = -Lbson/ -lpthread -lbson -lprotobuf -lmysqlclient -luuid

SRC_FILE=account_manager.cpp	\
	bson/bson_parser.cpp	\
	bson_buffer.cpp	\
	buffer.cpp	\
	daemon.cpp	\
	epoll_server.cpp \
	game_server.cpp	\
	game_template_manager.cpp	\
	log.cpp		\
	item.cpp	\
	stats_attr.cpp	\
	login_servlet.cpp	\
	role_servlet.cpp	\
	chat_servlet.cpp	\
	friend_servlet.cpp	\
	item_servlet.cpp	\
	gem_servlet.cpp		\
	skill_servlet.cpp	\
	task_servlet.cpp	\
	scene_servlet.cpp	\
	scene.cpp	\
	scene_manager.cpp	\
	task_servlet.cpp \
	soul_item_servlet.cpp	\
	main.cpp	\
	timer.cpp	\
	thread.cpp	\
	ring_queue.cpp	\
	session.cpp	\
	server_resource.cpp	\
	servlet.cpp	\
	socket.cpp	\
	socket_server.cpp	\
	mysql/dbconnection.cpp	\
	mysql/dbconnection_pool.cpp	\
	warrior.pb.cc	\
	peer/peer.pb.cc \
	peer/server_manager.cpp	\
	player_session.cpp	\
	player_session_manager.cpp	\
	item_bag.cpp	\
	souls_bag.cpp	\
	role.cpp	\
	role_manager.cpp	\
	task.cpp	\
	dataset/csv_reader.cpp	\
	dataset/role_attribute_template.cpp	\
	dataset/task_template_manager.cpp	\
	dataset/item_template_manager.cpp	\
	dataset/scene_template_manager.cpp	\
	dataset/skill_template_manager.cpp	\
	dataset/soul_item_template.cpp	\
	util.cpp

OBJ = account_manager.o    \
	bson/CBson.o  \
	bson/CBsonObject.o    \
	bson_buffer.o \
	buffer.o  \
	daemon.o  \
	epoll_server.o \
	game_server.o \
	game_template_manager.o   \
	log.o     \
	chat_servlet.o    \
	login_servlet.o   \
	item_servlet.o    \
	role_servlet.o    \
	skill_servlet.o	\
	scene.o	\
	scene_servlet.o	\
	scene_servlet.o   \
	task_servlet.o \
	main.o    \
	timer.o   \
	thread.o  \
	ring_queue.o  \
	session.o \
	server_resource.o \
	servlet.o \
	socket.o  \
	socket_server.o   \
	mysql/dbconnection.o  \
	mysql/dbconnection_pool.o \
	peer/peer.pb.o \
	peer/server_manager.o \
	player_session.o  \
	player_session_manager.o  \
	item_bag.o    \
	role.o    \
	role_manager.o    \
	task.o    \
	dataset/csv_reader.o  \
	dataset/role_attribute_template.o \
	dataset/task_template_manager.o   \
	dataset/item_template_manager.o   \
	dataset/skill_template_manager.o  \
	util.o

warrior: $(SRC_FILE)
	g++ -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm  $(OBJ)
