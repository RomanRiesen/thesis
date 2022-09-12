// ======================================================================
// \title  RocketSensor.hpp
// \author Roman Riesen
// \brief  hpp file for RocketSensor component implementation class
// ======================================================================

#ifndef RocketSensor_HPP
#define RocketSensor_HPP

#include "Ref/RocketSensor/RocketSensorComponentAc.hpp"

namespace Ref {

  class RocketSensor :
    public RocketSensorComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object RocketSensor
      //!
      RocketSensor(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object RocketSensor
      //!
      void init(
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object RocketSensor
      //!
      ~RocketSensor();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for rocketStatePort
      //!
      void rocketStatePort_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          F32 speedD, 
          F32 speedI, 
          F32 exhaustVelocity, 
          const Ref::RocketStage &stage, 
          F32 height 
      );

      //! Handler implementation for schedIn
      //!
      void schedIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          NATIVE_UINT_TYPE context /*!< 
      The call order
      */
      );


    };

} // end namespace Ref

#endif
