aux_source_directory(. SRC_LIST)
aux_source_directory(./db DB_LIST)

# 指定生成可执行文件
add_executable(ChatServer ${SRC_LIST} ${DB_LIST})

#指定可执行文件链接时需要的库文件
target_link_libraries(ChatServer nlohmann_json::nlohmann_json muduo_net muduo_base mysqlclient pthread)