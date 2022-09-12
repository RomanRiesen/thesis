// ======================================================================
// \title  RocketSimulator.hpp
// \author Roman Riesen
// \brief  hpp file for RocketSimulator component implementation class
// ======================================================================

#ifndef RocketSimulator_HPP
#define RocketSimulator_HPP

#define BASE_SIMULATION_DELAY 1000

#include "Ref/RocketSimulator/RocketSimulatorComponentAc.hpp"

//considering this is only used for simulation these libraries were considered good enough
#include <random>
#include <Fw/Types/SerialBuffer.hpp>

namespace Ref {

  class RocketSimulator :
    public RocketSimulatorComponentBase
  {

    //TODO add noise to initial state
    F32 t = 0; //[s]
    NATIVE_UINT_TYPE rate_group_dt = BASE_SIMULATION_DELAY; //[ms]
    const F32 dt = float(rate_group_dt / 1000.0);//[s] //should be (basecycle * (the rategroup factor)). This is not easily calculated here, thus left as a literal to be calculated manually.
    F32 a_r = 0;//[m/s^2]
    F32 speedD = 0;//[m/s]
    F32 speedI = 0;//[m/s]
    F32 burnRate = 0;//[kg/s]
    const F32 burnRateStage1 = 50;//[kg/s]
    const F32 burnRateStage2 = 10;//[kg/s]
    F32 exhaustVelocity = 0;//[m/s]
    F32 exhaustVelocityStage1 = 2000;//[m/s] //https://history.nasa.gov/SP-4404/app-b8.htm#:~:text=Typical%20values%20of%20exhaust%20velocity,2000%20to%204500%20m%2Fs.
    F32 exhaustVelocityStage2 = 2500;//[m/s]
    F32 remainingFuel1 = 300;//kg
    F32 remainingFuel2 = 100;//kg
    F32 gravity = -10;//[m/s^2]
    Ref::RocketStage stage = Ref::RocketStage::Prelaunch;
    const F32 empty_rocket_mass = 200;

    F32 real_h = 0;//[m]
    F32 real_v = 0;//[m/s]
    F32 real_a = 0;//[m/s^2]

    std::random_device rd{};
    std::mt19937 generator{rd()};//rd()

    std::normal_distribution<> e_a {0.0, 0.01};
    std::normal_distribution<> e_d {0.0, 0.5};
    std::normal_distribution<> e_b1 {0.0, 1};
    std::normal_distribution<> e_b2 {0.0, 0.5};

    bool is_running = false;

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object RocketSimulator
      //!
      RocketSimulator(
          const char *const compName /*!< The component name*/
      );

      //! Initialize object RocketSimulator
      //!
      void init(
          //const NATIVE_INT_TYPE queueDepth, /*!< The queue depth*/
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object RocketSimulator
      //!
      ~RocketSimulator();

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


      //sends all the current values of the rocket to telemetry and to the output ports
      void send_current_values();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Command handler implementations
      // ----------------------------------------------------------------------

      //! Implementation for TOGGLE_SIMULATION command handler
      //! 
      void TOGGLE_SIMULATION_cmdHandler(
          const FwOpcodeType opCode, /*!< The opcode*/
          const U32 cmdSeq /*!< The command sequence number*/
      );


    };

} // end namespace Ref

#endif
