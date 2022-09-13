// ======================================================================
// \title  ReelayMonitor.hpp
// \author Roman Riesen
// \brief  hpp file for ReelayMonitor component implementation class
// ======================================================================

#ifndef ReelayMonitor_HPP
#define ReelayMonitor_HPP

#include <algorithm>

#include "Ref/ReelayComponents/ReelayMonitor/ReelayMonitorComponentAc.hpp"
#include <./reelay/include/reelay/monitors.hpp>


#include <string>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <stdlib.h>

//Whether to use dense or discrete timed monitors (1 = discrete, 0 = dense)
#define USE_DISCRETE_TIMED 1

namespace ReelayComponents {

  using input_t = reelay::json;
  using output_t = reelay::json;
  using time_t = U32;
  using ReelayMonitorType = reelay::monitor<input_t, output_t>;

  class ReelayMonitor :
    public ReelayMonitorComponentBase
  {

    std::unordered_map<U32, std::pair<std::string, ReelayMonitorType>> properties = {};

    #if USE_DISCRETE_TIMED == 1
    reelay::discrete_monitor_options<time_t, input_t, output_t> monitor_options;
    #else
    reelay::dense_monitor_options<time_t, input_t, output_t> monitor_options;
    #endif

    bool only_log_negative_verdicts = false;

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object ReelayMonitor
      //!
      ReelayMonitor(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object ReelayMonitor
      //!
      void init(
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object ReelayMonitor
      //!
      ~ReelayMonitor();
    

      U32 add_property(Fw::String property, U32 unique_id, bool added_by_cmd = false);

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for eventIn
      //!
      void eventIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          const ReelayComponents::ReelayEvent &reelay_event 
      );

      //! Handler implementation for newProperty
      //!
      U32 newProperty_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          const ReelayComponents::ReelayProperty &property 
      );

      //! Handler implementation for removeProperty
      //!
      void removeProperty_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          U32 property_id 
      );


    };

} // end namespace Ref

#endif
