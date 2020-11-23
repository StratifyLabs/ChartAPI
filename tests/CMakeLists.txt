

set(DEPENDENCIES TestAPI ChartAPI FsAPI JsonAPI)

api_add_test_executable(${PROJECT_NAME} 32768 "${DEPENDENCIES}")



