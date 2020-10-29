
if(NOT DEFINED IS_SDK)
	include(JsonAPI)
	sos_sdk_include_target(ChartAPI "${STRATIFYAPI_CONFIG_LIST}")
endif()
