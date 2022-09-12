// ======================================================================
// \title  RocketSensor.cpp
// \author Roman Riesen
// \brief  cpp file for RocketSensor component implementation class
// ======================================================================


#include <Ref/RocketSensor/RocketSensor.hpp>
#include "Fw/Types/BasicTypes.hpp"

#include <sstream>

//include reelay to include the nlohman json library (not my proudest moment but hey, if it works it works)
#include "../ReelayMonitor/reelay/include/reelay/monitors.hpp"

namespace Ref {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  RocketSensor ::
    RocketSensor(
        const char *const compName
    ) : RocketSensorComponentBase(compName)
  {

  }

  void RocketSensor ::
    init(
        const NATIVE_INT_TYPE instance
    )
  {
    RocketSensorComponentBase::init(instance);
  }

  RocketSensor ::
    ~RocketSensor()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void RocketSensor ::
    rocketStatePort_handler(
        const NATIVE_INT_TYPE portNum,
        F32 speedD,
        F32 speedI,
        F32 exhaustVelocity,
        const Ref::RocketStage &stage,
        F32 height
    )
  {
    reelay::json event;
    //assign the rocket state values to the json object
    Fw::String stage_fw_str;
    stage.toString(stage_fw_str);
    std::string stage_str = stage_fw_str.toChar();
    event["v_I"] = speedD;
    event["v_D"] = speedI;	
    event["b"] = exhaustVelocity;
    event["s"] = stage_str;
    //manually writing the json has the advantage that we could limit the floating point precision thus saving precious
    // space (there's seemingly no support for that directly in nlohman).
    //std::string event_str = 
      //"{\"v_I\": " + std::to_string(speedD)
      //+ ", \"v_D\": " + std::to_string(speedI)
      //+ ", \"b\": " + std::to_string(exhaustVelocity)
      //+ ", \"stage\":" + stage_str
      //+ "\", \"height\": " + std::to_string(height)
      //+ "}";

    std::string event_str = event.dump();
    if (event_str.size() >= stage_fw_str.getCapacity()) {
      ///"Event string too large!"
      //FW_ASSERT(0);
    }
    Fw::String event_fw_str = event_str.c_str();
    ReelayComponents::ReelayEvent rlevent(event_fw_str); 

    this->tlmWrite_reelayEvent(event_fw_str);
    if (this->isConnected_reelayEventOut_OutputPort(0))
      this->reelayEventOut_out(0, rlevent);
  }

  void RocketSensor ::
    schedIn_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    // TODO
  }

} // end namespace Ref
