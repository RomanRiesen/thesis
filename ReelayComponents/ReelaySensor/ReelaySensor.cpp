// ======================================================================
// \title  ReelaySensor.cpp
// \author Roman Riesen
// \brief  cpp file for ReelaySensor component implementation class
// ======================================================================

#include <Ref/ReelayComponents/ReelaySensor/ReelaySensor.hpp>
#include "Fw/Types/BasicTypes.hpp"

namespace ReelayComponents
{

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  ReelaySensor ::
      ReelaySensor(
          const char *const compName) : ReelaySensorComponentBase(compName)
  {
  }

  void ReelaySensor ::
      init(
          // const NATIVE_INT_TYPE queueDepth,
          const NATIVE_INT_TYPE instance)
  {
    ReelaySensorComponentBase::init(/*queueDepth,*/ instance);
    serial_port_types.resize(getNum_serialIn_InputPorts());
    latest_values.resize(getNum_serialIn_InputPorts(), nullptr);
  }

  // ----------------------------------------------------------------------
  // Defining a serialIn connection's type and name 
  // ----------------------------------------------------------------------
  void ReelaySensor::set_data_channel_name(U32 portIndex, const std::string &valueName, const ReelayComponents::ReelayDataType &valueType)
  {
    std::pair<ReelayComponents::ReelayDataType, std::string> p = std::make_pair(valueType, valueName);
    serial_port_types[portIndex] = p;
  }

  ReelaySensor ::
      ~ReelaySensor()
  {
    for (Fw::SerialBuffer* v : latest_values) if (v != nullptr) delete v;
  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void ReelaySensor ::
      schedIn_handler(
          const NATIVE_INT_TYPE portNum,
          NATIVE_UINT_TYPE context)
  {
    // TODO use stream or strcopy or std::format (c++20) or something that isn't quite as awfully inefficient to construct a string (stringstream)
    // this->doDispatch();
    //Fw::String ss = R"({"time": 5, "status":"Prelaunch"})";
    //auto rlevv = Ref::ReelayEvent(ss);
    //this->tlmWrite_reelayEvent(ss);
    //this->reelayEventOut_out(0, rlevv);

    //return;//!!!!!

    std::string json_str = "";

    for (int i = 0; i < getNum_serialIn_InputPorts(); i++)
    {

      Fw::SerialBuffer* buffer_ptr = latest_values[i];

      if (buffer_ptr == nullptr)
      {
        this->log_ACTIVITY_LO_NO_VALUE_PRESENT(i);
        continue;
      }
      auto p = serial_port_types[i];
      auto port_type = p.first.e;
      std::string value_name = p.second;
      std::string value = "";
      Fw::SerializeStatus status = Fw::SerializeStatus::FW_SERIALIZE_OK;

      switch (port_type)
      {
        //port NumberValue(value: F32)
        //port StringValue(value: string)
        //port BoolValue(value: bool)
        //port ReelayEventValue(value: string)
        case ReelayComponents::ReelayDataType::Bool:
        {
          bool b;
          status = buffer_ptr->deserialize(b);
          value = b ? "true" : "false";
          break;
        }
        case ReelayComponents::ReelayDataType::Number:
        {
          F32 f;/// !!! !!! //FIXME //FIXME the actual port type is F32Value not F32...is that an issue??
          status = buffer_ptr->deserialize(f);
          value = std::to_string(f);
          break;
        }
        case ReelayComponents::ReelayDataType::String:
        {
          Fw::String s;
          status = buffer_ptr->deserialize(s);
          std::string str = s.toChar();
          value = "'" + str + "'";
          break;
        }
        case ReelayComponents::ReelayDataType::ReelayEvent:
        {
          ReelayComponents::ReelayEvent ev;
          status = buffer_ptr->deserialize(ev);
          value = ev.getevent_content().toChar();
          break;
        }
        default: { //Bottom type
          this->log_ACTIVITY_HI_UNKNOWN_TYPE(port_type, i);
        }
    }

      if (status != Fw::SerializeStatus::FW_SERIALIZE_OK)
      {
        // FIXME //TODO figure out what to do here.
        // something could not be serialized
        FW_ASSERT(false);
      }
      json_str += "'" + value_name + "':" + value + ",";
    }

    json_str = "{" + json_str + "}";


    Fw::String s = json_str.c_str();
    if (json_str.size() > s.getCapacity()) {
      // the produced string is too large to fit in the F' string
      FW_ASSERT(false);
    }
    auto rlev = ReelayComponents::ReelayEvent(s);
    this->tlmWrite_reelayEvent(s);

    if (this->isConnected_reelayEventOut_OutputPort(0))
      this->reelayEventOut_out(0, rlev);
  }


  //FIXME probably should be a command also
  void ReelaySensor ::
      setDataChannelName_handler(
          const NATIVE_INT_TYPE portNum,
          U32 portIndex,
          const valueNameString &valueName,
          const ReelayComponents::ReelayDataType &t)
  {
    set_data_channel_name(portIndex, valueName.toChar(), t);
  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined serial input ports
  // ----------------------------------------------------------------------

  void ReelaySensor ::
      serialIn_handler(
          NATIVE_INT_TYPE portNum,        /*!< The port number*/
          Fw::SerializeBufferBase &Buffer /*!< The serialization buffer*/
      )
  {
    // create json string
    // emit json string as event
    // https://nasa.github.io/fprime/Tutorials/GpsTutorial/Tutorial.html
    // the desired format is {"valueName":value} where value is either string or

    // c++ 20: [&Buffer]<typename T>(const auto& in){T v; Buffer.deserialize(v); return std::to_string(v);};

    //Buffer.copyRaw(*latest_values[portNum], Buffer.getBuffLength());

    //clean previous value
    if (latest_values[portNum] != nullptr)
      delete latest_values[portNum];

    latest_values[portNum] = new Fw::SerialBuffer(Buffer.getBuffAddr(), Buffer.getBuffLength());

    //latest_values[portNum]->setBuff(Buffer.getBuffAddr(), Buffer.getBuffLength());



  } // end namespace Ref
}
