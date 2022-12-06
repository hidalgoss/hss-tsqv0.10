# Create Library
#---------------------------------------------------#
ADD_LIBRARY(hss_ts_queue SHARED 
${CMAKE_CURRENT_SOURCE_DIR}/include/hss_ts_queue.hpp
${CMAKE_CURRENT_SOURCE_DIR}/include/hss_iqueue.hpp
)

TARGET_INCLUDE_DIRECTORIES(hss_ts_queue
PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/include
)

SET_TARGET_PROPERTIES(hss_ts_queue PROPERTIES LINKER_LANGUAGE CXX)
