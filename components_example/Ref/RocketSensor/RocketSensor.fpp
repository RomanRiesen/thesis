module Ref {

 
    @ component for receiving Data which is then sent out as ReelayEvent
    passive component RocketSensor {
        
        @ Used by the rateGroup to set the sensor baudrate
        sync input port schedIn: Svc.Sched

        output port reelayEventOut : ReelayComponents.EventIn

        sync input port rocketStatePort : RocketStatePort


        @ Port for time
        time get port timeGetOut

        @ Port for events
        event port eventOut

        @ Port for text events
        text event port textEventOut

        @ Telemetry port
        telemetry port tlmOut

        telemetry reelayEvent: string id 0
    }
}