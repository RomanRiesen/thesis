include(autocoder/ai-shared)

set(CHEETAH_TEMPLATES
  "${CMAKE_CURRENT_LIST_DIR}/cpp.tmpl"
  "${CMAKE_CURRENT_LIST_DIR}/hpp.tmpl"
)

cheetah("${CHEETAH_TEMPLATES}")
