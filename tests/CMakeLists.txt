

set(DEPENDENCIES TestAPI ChartAPI FsAPI JsonAPI)

stratify_api_add_test_executable(ChartAPI 32768 "${DEPENDENCIES}")



