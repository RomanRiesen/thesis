set(SOURCE_FILES
    "${CMAKE_CURRENT_LIST_DIR}/ReelaySensor.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/ReelaySensor.fpp"
)

register_fprime_module()


set(UT_SOURCE_FILES
    #"${CMAKE_CURRENT_LIST_DIR}/ReelaySensor.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/ReelaySensor.fpp"
)

register_fprime_ut()