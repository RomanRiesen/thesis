# ======================================================================
# CMakeLists.txt
# ======================================================================

# We need to declare the XML source files this way to invoke the autocoder.
# However, only the UT build is allowed here.
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/Component1ComponentAi.xml"
  "${CMAKE_CURRENT_LIST_DIR}/Port1PortAi.xml"
  "${CMAKE_CURRENT_LIST_DIR}/ExampleTypeSerializableAi.xml"
)
register_fprime_module()

# Declare dependencies on test modules
set(UT_MOD_DEPS
  Fw/Test
  STest
)

# List all .cpp files as UT_SOURCE_FILES. Only the UT build is allowed.
set(UT_SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/Component1.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/AllTypes.cpp"

)
register_fprime_ut()
