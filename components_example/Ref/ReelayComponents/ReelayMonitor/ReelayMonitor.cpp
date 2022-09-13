// ======================================================================
// \title  ReelayMonitor.cpp
// \author Roman Riesen
// \brief  cpp file for ReelayMonitor component implementation class
// ======================================================================

#include <Ref/ReelayComponents/ReelayMonitor/ReelayMonitor.hpp>
#include "Fw/Types/BasicTypes.hpp"

#include <iostream>

namespace ReelayComponents
{

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  ReelayMonitor ::
      ReelayMonitor(
          const char *const compName) : ReelayMonitorComponentBase(compName)
  {

    //construct the appropriate monitor options
    #if USE_DISCRETE_TIMED == 1
    monitor_options = reelay::discrete_timed<time_t>::monitor<input_t, output_t>::options()
      //.with_time_field_name("time")
      //.with_condensing(true)
      .disable_condensing()
      .with_value_field_name("verdict");
    #else
    monitor_options = reelay::dense_timed<time_t>::monitor<input_t, output_t>::options()
      //.with_interpolation()
      .with_time_field_name("time")
      .with_value_field_name("verdict");
    #endif
  }

  void ReelayMonitor ::
      init(
          const NATIVE_INT_TYPE instance)
  {
    ReelayMonitorComponentBase::init(instance);
  }

  ReelayMonitor ::
      ~ReelayMonitor()
  {
  }

  U32 ReelayMonitor::add_property(Fw::String property, U32 unique_id, bool added_by_cmd)
  {
    ReelayMonitorType mon;
    std::string prop = property.toChar();
    try
    {
      //from the property string, construct a monitor
      mon = reelay::make_monitor(prop, monitor_options);
    }
    catch (const std::exception &exception)
    {
      // do not throw if the command was added at runtime but rather only issue high priority warning
      this->log_ACTIVITY_HI_INVALID_PROPERTY_STRING(ReelayComponents::ReelayProperty(property, unique_id));
      if (!added_by_cmd)
      {
        //maybe should be fatal event, not just throw.
        throw exception;
      }
      return unique_id;
    }

    properties.emplace(unique_id, std::make_pair(prop, mon));
    return unique_id;
  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void ReelayMonitor ::
      eventIn_handler(
          const NATIVE_INT_TYPE portNum,
          const ReelayComponents::ReelayEvent &reelay_event)
  {

    if (properties.empty())
    {
      this->log_ACTIVITY_LO_NO_PROPERTY_REGISTERED();
      return;
    }

    std::string evc = reelay_event.getevent_content().toChar();
    std::cout << evc << std::endl;

    // handle json parsing errors
    reelay::json msg;
    try
    {
      msg = reelay::json::parse(evc);
    }
    catch (const std::exception &exception)
    {
      this->log_ACTIVITY_HI_INVALID_EVENT_STRING(reelay_event);
    }

    if (msg.empty())
    {
      this->log_ACTIVITY_HI_INVALID_EVENT_STRING(reelay_event);
      return;
    }


    Fw::Time t = this->getTime();
    F32 ms = t.getSeconds() * 1000 + t.getUSeconds() / 1000;

    //add time field to json message if not present
    if (!msg.contains("time"))
    {
      msg["time"] = ms;
    }

    std::cout << "event: " << msg.dump() << std::endl;

    bool all_positive = true;
    // iterate over all properties and check if they are satisfied by the event
    for (auto &it : properties)
    {

      uint32_t property_id = it.first;

      //get the verdict (array) from the monitor
      auto verdict = it.second.second.update(msg);
      std::cout << "verdict : " << verdict << std::endl;

      #if USE_DISCRETE_TIMED == 1

      // in the case of a discrete monitor the verdict is a simple bool
      bool result = verdict["verdict"];

      #else

      //in the case of a dense monitor the verdict is a list of times and verdicts
      bool result = true;
      if (!verdict.empty())
      {
        //each segment represents a point in time in which a change happened
        for (const auto& s : verdict)
        {
          std::cout << s.dump() << std::endl;
          bool this_result = s["verdict"].get<bool>(); 
          //if at any time the verdict is negative the entire result is negative
          result &= this_result;
        }
      }

      #endif


      //always output the simple bool verdict
      if (this->isConnected_verdictOutBool_OutputPort(0))
        this->verdictOutBool_out(0, result);

      if (!result /*&& !only_log_negative_verdicts*/)
      {
        all_positive = false;
        ReelayProperty::propertyString property_string = it.second.first.c_str();
        ReelayProperty violated_property(property_string, property_id);
        this->log_ACTIVITY_HI_NEGATIVE_VERDICT(violated_property);
      }
    }

    if (all_positive)
    {
      this->log_ACTIVITY_LO_ALL_PROPERTIES_POSITIVE();
    }

  }

  U32 ReelayMonitor ::
      newProperty_handler(
          const NATIVE_INT_TYPE portNum,
          const ReelayComponents::ReelayProperty &property)
  {
    //OPTI many allocations happening.
    return add_property(property.getproperty(), property.getunique_id(), true);
  }

  void ReelayMonitor ::
      removeProperty_handler(
          const NATIVE_INT_TYPE portNum,
          U32 property_id)
  {
    properties.erase(property_id);
  }

} // end namespace Ref
