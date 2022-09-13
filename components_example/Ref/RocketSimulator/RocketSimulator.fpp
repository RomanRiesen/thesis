module Ref {

    port RocketStatePort (speedD: F32, speedI: F32, exhaustVelocity: F32, stage: RocketStage, height: F32)

    port F32Value(value: F32)
    port StringValue(value: string)
    port BoolValue(value: bool)
    port ReelayEventValue(value: string)

    struct RocketState {
        v_I: F32,
        v_Delta: F32,
        burnrate: F32,
        stage: string,
        height: F32,
    }

    enum RocketStage {
        Prelaunch,
        Ignition,
        FirstStage,
        SecondStage,
        Descent,
        Landed
    }

    port RocketStage (
        stage: RocketStage
    )


    @ component for generating data to be verified later
    passive component RocketSimulator {

        @ Command receive port
        command recv port cmdIn

        @ Command registration port
        command reg port cmdRegOut

        @ Command response port
        command resp port cmdResponseOut


        @ Telemetry port
        telemetry port tlmOut

        #@ event port
        #event port eventOut

        #@ Text event port #somehow produces a link time error in the unit test.
        #text event port textEventOut

        @ Time get
        time get port timeGetOut

        sync command TOGGLE_SIMULATION()
        sync input port schedIn: Svc.Sched

        # These ports would be used by the generic reelay sensor
        #output port speedD : F32Value
        #output port speedI : F32Value
        #output port rocket_t : F32Value
        #output port exhaustVelocity : F32Value
        #output port stage : StringValue
        #output port height : F32Value

        output port all_values : RocketStatePort

        #sync input port speedD : RF32Value
        #sync input port speedI : RF32Value
        #sync input port rocket_t : RF32Value
        #sync input port exhaustVelocity : RF32Value
        #sync input port stage : RStringValue
        #sync input port height : RF32Value

        telemetry v_I: F32 id 0
        telemetry v_Doppler: F32 id 1
        telemetry v_Delta: F32 id 2
        telemetry rocket_time: F32 id 3
        telemetry burnrate: F32 id 4
        telemetry stage: Ref.RocketStage id 5
        telemetry height: F32 id 6
        telemetry acceleration: F32 id 7
        telemetry remaining_fuel: F32 id 8
        telemetry exhaustVelocity: F32 id 9
    }
}
