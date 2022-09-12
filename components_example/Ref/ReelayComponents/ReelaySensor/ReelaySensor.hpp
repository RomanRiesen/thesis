// ======================================================================
// \title  ReelaySensor.hpp
// \author Roman Riesen
// \brief  hpp file for ReelaySensor component implementation class
// ======================================================================

#ifndef ReelaySensor_HPP
#define ReelaySensor_HPP

#include "Ref/ReelayComponents/ReelaySensor/ReelaySensorComponentAc.hpp"

#include "Fw/Types/SerialBuffer.hpp"
#include <vector>
#include <string>

namespace ReelayComponents {

  /*

    Like the ReelayTypes/Json type, this component would represent a significant improvement to the (re)usability of our components.

  */
  class ReelaySensor :
    public ReelaySensorComponentBase
  {

    std::vector<std::pair<ReelayComponents::ReelayDataType, std::string>> serial_port_types;
    std::vector<Fw::SerialBuffer*> latest_values;

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object ReelaySensor
      //!
      ReelaySensor(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object ReelaySensor
      //!
      void init(
          //const NATIVE_INT_TYPE queueDepth, /*!< The queue depth*/
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );


      void set_data_channel_name(U32 portIndex, const std::string& valueName, const ReelayComponents::ReelayDataType &valueType);

      //! Destroy object ReelaySensor
      //!
      ~ReelaySensor();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for schedIn
      //!
      void schedIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          NATIVE_UINT_TYPE context /*!< 
      The call order
      */
      );
      //! Handler implementation for setDataChannelName
      //!
      //FIXME one of portIndex or portNum would suffice
      void setDataChannelName_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          U32 portIndex, 
          const valueNameString &valueName, 
          const ReelayComponents::ReelayDataType &t 
      );

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined serial input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for serialIn
      //!
      void serialIn_handler(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        Fw::SerializeBufferBase &Buffer /*!< The serialization buffer*/
      );


    };

} // end namespace Ref

#endif
