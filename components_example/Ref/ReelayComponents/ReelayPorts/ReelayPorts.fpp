
module ReelayComponents {


    @ Port for creating Properties. Returns the identifying index of the newly created property.
    port ReelayCreateProperty (
        property : ReelayProperty
    ) -> U32

    @ Port for deleting Properties
    port ReelayDeleteProperty (
        property_id : U32
    )

    @ Port for receiving reelay Events TODO this should be more generic, really. Maybe I'll have to manually implement the component/Port in cpp so it can be generic??
    port EventIn (
        reelay_event : ReelayEvent
    )

    @ Port for sending the verdict
    port ReelayVerdictOut (
        verdict : bool,
        property_id : U32
    )

    port BoolPort (
        val : bool
    )


    @ Port for setting data channel name (used to transform the untyped streams into reelay events inside of ReelaySensor)
    port DataChannelName (
        portIndex : U32,
        valueName : string,
        t : ReelayDataType
    )



    #port RF32Value() -> F32
    #port RStringValue() -> string
    #port RBoolValue() -> bool
    #port RReelayEventValue() -> string

}