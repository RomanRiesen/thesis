// ======================================================================
// \title  RocketSimulator.cpp
// \author Roman Riesen
// \brief  cpp file for RocketSimulator component implementation class
// ======================================================================


#include <Ref/RocketSimulator/RocketSimulator.hpp>
#include "Fw/Types/BasicTypes.hpp"

//considering this is only used for simulation these libraries were considered good enough
#include <random>
#include <chrono>

namespace Ref {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  RocketSimulator ::
    RocketSimulator(
        const char *const compName
    ) : RocketSimulatorComponentBase(compName)
  {

  }

  void RocketSimulator ::
    init(
      const NATIVE_INT_TYPE queueDepth//,
      //const NATIVE_INT_TYPE instance
    )
  {
    RocketSimulatorComponentBase::init(queueDepth/*, instance*/);
    generator.seed(42);
  }

  RocketSimulator ::
    ~RocketSimulator()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void RocketSimulator ::
    schedIn_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    if (!is_running) {send_current_values(); return;}

    //increase time by dt
    t += dt;//[s]

    //get a normally distributed value from the random variable
    F32 error_a = e_a(generator);
    F32 error_d = e_d(generator);
    F32 error_b1 = e_b1(generator);
    F32 error_b2 = e_b2(generator);

    bool first_stage_over = remainingFuel1 <= burnRateStage1*dt;
    bool second_stage_over = remainingFuel2 <= burnRateStage2*dt;

    if (t > 0.5) {
      stage = Ref::RocketStage::FirstStage;
    }
    //TODO calculate thrown-away momentum lost due to ditching before fully empty?
    if (t > 90 || first_stage_over) {
      stage = Ref::RocketStage::SecondStage;
    }
    if (t > 500 || second_stage_over) {
      stage = Ref::RocketStage::Descent;
      burnRate = 0;
      exhaustVelocity = 0;
    }

    bool has_landed = real_h <= std::abs(real_v * dt) && first_stage_over && second_stage_over;
    if (has_landed) {
      stage = Ref::RocketStage::Landed;
      real_v = 0;//hit ground 
      real_h = 0;
    }

    if (stage == Ref::RocketStage::FirstStage)
    {
      burnRate = (burnRateStage1+error_b1);
      remainingFuel1 -= burnRate*dt;
      exhaustVelocity = exhaustVelocityStage1;
    }
    if (stage == Ref::RocketStage::SecondStage)
    {
      burnRate = (burnRateStage2+error_b2);
      remainingFuel2 -= burnRate*dt;
      exhaustVelocity = exhaustVelocityStage2;
    }

    F32 momentum = burnRate*exhaustVelocity;//kg m / s^2

    F32 rocket_mass = empty_rocket_mass + remainingFuel1 + remainingFuel2;

    real_a = momentum / rocket_mass + gravity;//m/s^2

    //Left Riemann Sum integration

    //can only change speed if off of launch platform.
    if (stage != Ref::RocketStage::Prelaunch && stage != Ref::RocketStage::Landed && !has_landed) {
      real_v += real_a*dt;
    }
    F32 acceleormeter_a = real_a + error_a;
    real_h += real_v*dt;
    speedI += (acceleormeter_a)*dt;
    speedD = real_v + error_d;

    send_current_values();
  }


  void RocketSimulator::send_current_values() {
    Fw::String stage_str;
    stage.toString(stage_str);

    F32 v_Delta = std::abs(speedI-speedD);

    // Send current values to output ports
    //FIXME crashes!!!
    if (
      this->isConnected_height_OutputPort(0)
      || this->isConnected_speedD_OutputPort(0)
      || this->isConnected_speedI_OutputPort(0)
      || this->isConnected_stage_OutputPort(0)
      || this->isConnected_rocket_t_OutputPort(0)
    )
    {
      this->speedD_out(0, speedI);
      this->speedI_out(0, v_Delta);
      this->exhaustVelocity_out(0, burnRate);
      this->stage_out(0, stage_str);
      this->height_out(0, real_h);
    }

    if (this->isConnected_all_values_OutputPort(0))
      this->all_values_out(0, speedI, v_Delta, exhaustVelocity, stage, real_h);

    this->tlmWrite_v_Delta(v_Delta);
    this->tlmWrite_v_I(speedI);
    this->tlmWrite_v_Doppler(speedD);
    this->tlmWrite_stage(stage);
    this->tlmWrite_burnrate(burnRate);
    this->tlmWrite_height(real_h);
    this->tlmWrite_remaining_fuel(remainingFuel1+remainingFuel2);
    this->tlmWrite_acceleration(real_a);
    this->tlmWrite_exhaustVelocity(exhaustVelocity);
    this->tlmWrite_rocket_time(t);
  }


  // ----------------------------------------------------------------------
  // Command handler implementations
  // ----------------------------------------------------------------------

  void RocketSimulator ::
    TOGGLE_SIMULATION_cmdHandler(
        const FwOpcodeType opCode,
        const U32 cmdSeq
    )
  {
    is_running = !is_running;
    send_current_values();
    this->cmdResponse_out(opCode,cmdSeq, Fw::CmdResponse::OK);
  }

} // end namespace Ref