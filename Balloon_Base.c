class Balloon_Base extends CarScript
{
// ---------- ERROR HANDLIMG -----------

    // ERROR HANDLE 
    bool m_IsError;
    bool m_IsDebug;
    bool m_IsDisconnected;

// ----------- MECHANICS --------------

	int m_balloon_state = 0;
    bool m_IsPlayerInside;

    // weather and wind related
    float m_WindSpeed;
    float m_WindDirection;
    float m_WindIntensity;

    // fuel - gas
    float m_GasLevel;
    float fuelLevel;
    bool m_IsGasOn;
    bool m_IsBurnerOn;
    bool m_IsCompressorOn;

    // pointless? 
    bool m_IsReversing;

// ----------- INPUT VARIABLES --------------

    //float m_Weight;
    bool m_IsLanded;
    bool m_IsGrounded;
    float m_Altitude;
    bool m_IsFlying;
    bool m_IsDescending;
    bool m_IsDescendingFast;
    bool m_IsDescendingSlow;

    // rotax input 
    bool m_IsAirbrake;
    bool m_IsForwardFlight;
    bool m_IsSteeringLeft;
    bool m_IsSteeringRight;

    // rotax add on  
    bool m_IsRotaxOn;
    bool m_IsRotaxAttached;
	int m_RotaxEngine;
	int m_FireParticles;

    // keypress
    int m_igniter_press = 0;
    int m_emergency_shutoff_press = 0;
    int m_gas_switch_toggle_press = 0;
    int m_compressor_toggle_press = 0;

    // ^ v
    int m_heat_increase_press = 0;
    int m_heat_decrease_press = 0;

    // rotax forward thrust
    int m_rotax_increase_press = 0;
    int m_rotax_decrease_press = 0;

    // rotax turning
    int m_rotax_left_press = 0;
    int m_rotax_right_press = 0;
	
	int m_heat_level = 0;
	int m_throttle_level = 0;

// ------------ CONFIG / CONSTANTS -------------

    // ! TODO: Eventually make these settings store 
    // and fetch from server side config

    const float GAS_CONSUMPTION_RATE = 1.0;
    const float MAXIMUM_GAS_LEVEL = 100.0;
    const float MINIMUM_GAS_LEVEL = 0.0;
    const float GAS_START_LEVEL = 50.0;
    const float MAXIMUM_WEIGHT = 1000.0;
    const float MINIMUM_WEIGHT = 100.0;

    const float MAXIMUM_ALTITUDE = 1000.0;
    const float MINIMUM_ALTITUDE = 0.0;
    const float MAXIMUM_WIND_SPEED = 20.0;
    const float MINIMUM_WIND_SPEED = 0.0;
    const float WIND_DIRECTION_MAX = 360.0;
    const float WIND_DIRECTION_MIN = 0.0;

    const float ROTAX_SPEED_MAX = 100.0;
    const float ROTAX_SPEED_MIN = 0.0;
    const float FORWARD_SPEED_MAX = 20.0;
    const float FORWARD_SPEED_MIN = 0.0;
    const float STEERING_SPEED_MAX = 20.0;
    const float STEERING_SPEED_MIN = 0.0;

// ------------- SOUND AND PARTICLE EFFECTS ----------------

    ref Effect m_FireEffect, m_FireEffect2;
    ref EffVehicleSmoke m_blaster1Fx, m_blaster2Fx;
    int m_blaster1PtcFx, m_blaster2PtcFx;
    vector m_blaster1PtcPos, m_blaster2PtcPos;
    vector m_blaster1PtcDir, m_blaster2PtcDir;

    ref SoundParams m_SoundParams;
    ref SoundObjectBuilder m_SoundBuilder;
    ref SoundObject m_CompressorSound;
    ref SoundObject m_GasFlowSound;
    ref SoundObject m_IgniterSound;
    ref SoundObject m_LowGasSound;
    ref SoundObject m_GasBlowOffSound;
    ref SoundObject m_InFlightSound;
    ref SoundObject m_WindSound;
    ref SoundObject m_BurnerFireSound;


	
    bool m_LandingLights;
    static const int SELECTION_ID_LANDING_LIGHTS = 10;
    PointLightBase m_landing_light_0;
    vector m_landing_light_0_PtcPos;
    bool m_InteriorLights;
    static const int SELECTION_ID_INTERIOR_LIGHT = 9;
    PointLightBase m_landing_light_3;
    vector m_landing_light_3_PtcPos;
    static string m_landing_light_3_point = "landing_light_rear";

// --------------------------------------------
// -------------- BASE CLASS ------------------
// --------------------------------------------


    void Balloon_Base()
    {
        //IsBalloon();

        // debug
        m_IsError = false;
        m_IsDisconnected = false;
        m_IsDebug = false;

        // weather
        m_WindSpeed = MINIMUM_WIND_SPEED;
        m_WindDirection = WIND_DIRECTION_MIN;


        // fuel
        m_GasLevel = GAS_START_LEVEL;
        m_IsGasOn = false;
        m_IsBurnerOn = false;

        // rotax input
        m_IsForwardFlight = false;
        m_IsAirbrake = false;
        m_IsSteeringLeft = false;
        m_IsSteeringRight = false;

        // rotax
        m_IsRotaxAttached = false;
        m_IsRotaxOn = false;

        // simulation
        m_Weight = MINIMUM_WEIGHT;
        m_IsFlying = false;
        m_IsLanded = true;
        m_IsGrounded = true;
        m_Altitude = MINIMUM_ALTITUDE;
        m_IsDescending = false;
        m_IsDescendingFast = false;
        m_IsDescendingSlow = false;
        // !!
        m_IsReversing = false;
        m_RotaxEngine = 0;
        m_FireParticles = 0;
        m_WindIntensity = 0;
        m_IsCompressorOn = false;

		SetEventMask( EntityEvent.CONTACT | EntityEvent.SIMULATE | EntityEvent.POSTSIMULATE | EntityEvent.POSTFRAME );
	
    	//dBodyActive( this, ActiveState.ACTIVE );
		dBodyActive( this, ActiveState.ALWAYS_ACTIVE );
		dBodyDynamic( this, true );

        // build sounds
        m_CarDoorOpenSound = "offroad_door_open_SoundSet";
        m_CarDoorCloseSound = "offroad_door_close_SoundSet";    
        m_SoundParams = new SoundParams("HotAirBalloon");
        m_SoundBuilder = new SoundObjectBuilder(m_SoundParams);

        RegisterNetSyncVariableBool("m_isBalloon");
        RegisterNetSyncVariableBool("m_IsError");
        RegisterNetSyncVariableBool("m_IsDebug");
        RegisterNetSyncVariableBool("m_IsDisconnected");
        RegisterNetSyncVariableBool("m_IsCompressorOn");
        RegisterNetSyncVariableBool("m_IsGasOn");
        RegisterNetSyncVariableBool("m_IsBurnerOn");
        RegisterNetSyncVariableBool("m_IsRotaxOn");
        RegisterNetSyncVariableBool("m_IsRotaxEngineAttached");
        RegisterNetSyncVariableBool("m_IsLanding");
        RegisterNetSyncVariableBool("m_IsPlayerInside");
        RegisterNetSyncVariableBool("m_IsReversing");
        RegisterNetSyncVariableBool("m_IsLanded");
        RegisterNetSyncVariableBool("m_IsGrounded");
        RegisterNetSyncVariableBool("m_IsFlying");
        RegisterNetSyncVariableBool("m_IsDescending");
        RegisterNetSyncVariableBool("m_IsDescendingFast");
        RegisterNetSyncVariableBool("m_IsDescendingSlow");
        RegisterNetSyncVariableBool("m_IsAirbrake");
        RegisterNetSyncVariableBool("m_IsForwardFlight");
        RegisterNetSyncVariableBool("m_IsSteeringLeft");
        RegisterNetSyncVariableBool("m_IsSteeringRight");
		RegisterNetSyncVariableFloat("m_GasLevel");
		RegisterNetSyncVariableFloat("m_Altitude");
		RegisterNetSyncVariableFloat("m_WindIntensity");
		RegisterNetSyncVariableFloat("m_WindSpeed");
		RegisterNetSyncVariableFloat("m_WindDirection");
		RegisterNetSyncVariableFloat("m_Weight");
		RegisterNetSyncVariableInt("m_balloon_state");
		RegisterNetSyncVariableInt("m_heat_increase_press");
		RegisterNetSyncVariableInt("m_heat_decrease_press");

        if (MemoryPointExists("ptc_fire_1_end"))
        {
            m_blaster1PtcPos = GetMemoryPointPos("ptc_fire_1_end");
            if (MemoryPointExists("ptc_fire_1_start"))
            {
                vector blaster1Start = GetMemoryPointPos("ptc_fire_1_start");
                vector tempOri1 = vector.Direction(blaster1Start, m_blaster1PtcPos);
                m_blaster1PtcDir[0] = -tempOri1[2];
                m_blaster1PtcDir[1] = tempOri1[1];
                m_blaster1PtcDir[2] = tempOri1[0];
                m_blaster1PtcDir = m_blaster1PtcDir.Normalized().VectorToAngles();
            }
        }

        if (MemoryPointExists("ptc_fire_2_end"))
        {
            m_blaster2PtcPos = GetMemoryPointPos("ptc_fire_2_end");
            if (MemoryPointExists("ptc_fire_2_start"))
            {
                vector blaster2Start = GetMemoryPointPos("ptc_fire_2_start");
                vector tempOri2 = vector.Direction(blaster2Start, m_blaster2PtcPos);
                m_blaster2PtcDir[0] = -tempOri2[2];
                m_blaster2PtcDir[1] = tempOri2[1];
                m_blaster2PtcDir[2] = tempOri2[0];
                m_blaster2PtcDir = m_blaster2PtcDir.Normalized().VectorToAngles();
            }
        }
    }

	/*
    void ~Ballon_Base()
    {
        if (m_blaster1PtcFx && SEffectManager.IsEffectExist(m_blaster1PtcFx)) 
			SEffectManager.Stop(m_blaster1PtcFx);

        if (m_blaster2PtcFx && SEffectManager.IsEffectExist(m_blaster2PtcFx)) 
			SEffectManager.Stop(m_blaster2PtcFx);
    } 
	*/ 

// ---------------- CONFIG -----------------


    /*
	void LoadBalloonConfiguration(string balloon_type, Hot_Air_Balloon_Config_Data m_HotAirBalloonConfigData )
	{
		float p_fuel_cap = m_HotAirBalloonConfigData.GetConfigData(balloon_type).getFuelCap();
		float p_fuel_rate = m_HotAirBalloonConfigData.GetConfigData(balloon_type).getFuelRate() / 60;
		float p_airspeed_max = m_HotAirBalloonConfigData.GetConfigData(balloon_type).getAirspeedMax();
		float p_MAXIMUM_ALTITUDE = m_HotAirBalloonConfigData.GetConfigData(balloon_type).getAltitudeMax();
		float p_climb_max = m_HotAirBalloonConfigData.GetConfigData(balloon_type).getClimbMax()/60;
		float p_aero_drag = (m_HotAirBalloonConfigData.GetConfigData(balloon_type).getAeroDrag() / 100) * 0.02;
		float p_heat_rate = (m_HotAirBalloonConfigData.GetConfigData(balloon_type).getBlasterHeatRate() / 100) * 0.47;
	}
    */

// ---------------- EFFECTS -----------------------

  override int GetAnimInstance()
    {
        return VehicleAnimInstances.S120;
    }

    // rpc related sound
    void PlaySound(ERPCs rpc)
    {
        if (!m_IsError && !m_IsDisconnected)
        {
            GetGame().RPCSingleParam(this, rpc, null, true);
        }
    }

		/*
    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);

        if (rpc_type == ExpansionHotAirBalloonRPCEnum.RPC_EXPANSION_HOT_AIR_BALLOON_ROTAX_ENGINE_SPEED)
        {
            float rotaxEngineSpeed = 0;
            ctx.Read(rotaxEngineSpeed);

            if (m_RotaxEngine)
            {
                m_RotaxEngine.SetAnimationPhase("rotax_engine_propeller", rotaxEngineSpeed);
            }
        }
	

        switch (rpc_type)
        {
            case RPC_INIT_HOT_AIR_BALLOON:
                OnRPCInitHotAirBalloon();
                break;

            case RPC_START_COMPRESSOR:
                OnRPCStartCompressor();
                break;

            case RPC_TURN_ON_GAS:
                OnRPCTurnOnGas();
                break;

            case RPC_IGNITE_BURNERS:
                OnRPCIgniteBurners();
                break;

            case RPC_TURN_LEFT:
                OnRPCTurnLeft();
                break;

            case RPC_TURN_RIGHT:
                OnRPCTurnRight();
                break;

            case RPC_MOVE_FORWARD:
                OnRPCMoveForward();
                break;

            case RPC_MOVE_BACKWARD:
                OnRPCMoveBackward();
                break;
        }
    }	
		*/
    void OnRPCInitHotAirBalloon()
    {
        // Activate air generator and trigger compressor sound
       // m_CompressorSound = GetGame().CreateSoundOnObject(this, "HotAirBalloonCompressor", 20, true);
        //m_CompressorSound.Play();
    }

    void OnRPCStartCompressor()
    {
        // Ensure smooth progression of balloon inflation and operation
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(StartGasFlow, 2000);
    }

    void StartGasFlow()
    {
       // m_CompressorSound.Stop();
       // m_GasFlowSound = GetGame().CreateSoundOnObject(this, "HotAirBalloonGasFlow", 20, true);
        //m_GasFlowSound.Play();
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(StartInflation, 2000);
    }

    void StartInflation()
    {
        SetAnimationPhase("source_balloonempty", 5);
        
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(WaitForPlayerAction, 5000);
    }


    private void WaitForPlayerAction()
    {
        // Wait for player action when the animation phase reaches 5
    }

    private void OnRPCTurnOnGas()
    {
        // Use another input key to turn on/open the gas line
        m_IsGasOn = true;

        // Trigger gas flow sound and gradually consume vehicle's fuel level
      //  m_GasFlowSound.Stop();
      //  m_LowGasSound = GetGame().CreateSoundOnObject(this, "HotAirBalloonLowGas", 20, true);

       // m_LowGasSound.Play();
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ConsumeFuel, 1000);
    }

    private void ConsumeFuel()
    {
        if (m_IsGasOn && m_GasLevel > MINIMUM_GAS_LEVEL)
        {
            m_GasLevel -= GAS_CONSUMPTION_RATE;

            GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ConsumeFuel, 1000);
        }
        else
        {
            m_IsGasOn = false;
           // m_LowGasSound.Stop();

            //m_GasBlowOffSound = GetGame().CreateSoundOnObject(this, "HotAirBalloonGasBlowOff", 20, true);
           // m_GasBlowOffSound.Play();
        }
    }

    private void OnRPCIgniteBurners()
    {
        // Use an additional input key to ignite the gas burners
        m_IsBurnerOn = true;

        // Play player igniter sound briefly and activate fire particles in the burners
      // m_IgniterSound = GetGame().CreateSoundOnObject(this, "HotAirBalloonIgniter", 20, true);
       // m_IgniterSound.Play();
      //  m_FireEffect = GetGame().CreateEffect("FireParticles", this, "", true);

        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(StartFlight, 500);
    }

    private void StartFlight()
    {
        // Set animation phases
        SetAnimationPhase("source_balloonempty", 1);
        SetAnimationPhase("source_hideballoonhalf", 0);
        SetAnimationPhase("source_balloonhalf", 0);
        
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ContinueFlight, 5000);
    }

    private void ContinueFlight()
    {
        SetAnimationPhase("source_hideballoonhalf", 1);
        SetAnimationPhase("source_hideballoonfull", 0);
        SetAnimationPhase("source_balloonempty", 0);
    }

    void StartCompressor()
    {
        m_IsCompressorOn = true;

        if (GetGame().IsServer())
        {
           // GetGame().RPCSingleParam(this, ExpansionHotAirBalloonRPCEnum.RPC_EXPANSION_HOT_AIR_BALLOON_START_COMPRESSOR, NULL, true);
        }
    }

    void StopCompressor()
    {
        m_IsCompressorOn = false;

        if (GetGame().IsServer())
        {
           // GetGame().RPCSingleParam(this, ExpansionHotAirBalloonRPCEnum.RPC_EXPANSION_HOT_AIR_BALLOON_STOP_COMPRESSOR, NULL, true);
        }
    }

    void StartGas()
    {
        m_IsGasOn = true;

        if (GetGame().IsServer())
        {
          //  GetGame().RPCSingleParam(this, ExpansionHotAirBalloonRPCEnum.RPC_EXPANSION_HOT_AIR_BALLOON_START_GAS, NULL, true);
        }
    }

    void StopGas()
    {
        m_IsGasOn = false;

        if (GetGame().IsServer())
        {
          //  GetGame().RPCSingleParam(this, ExpansionHotAirBalloonRPCEnum.RPC_EXPANSION_HOT_AIR_BALLOON_STOP_GAS, NULL, true);
        }
    }

    void StartBurner()
    {
        m_IsBurnerOn = true;

        if (GetGame().IsServer())
        {
          //  GetGame().RPCSingleParam(this, ExpansionHotAirBalloonRPCEnum.RPC_EXPANSION_HOT_AIR_BALLOON_START_BURNER, NULL, true);
        }
    }

    void StopBurner()
    {
        m_IsBurnerOn = false;

        if (GetGame().IsServer())
        {
          //  GetGame().RPCSingleParam(this, ExpansionHotAirBalloonRPCEnum.RPC_EXPANSION_HOT_AIR_BALLOON_STOP_BURNER, NULL, true);
        }
    }

		/*
    void AttachRotaxEngine(Object rotaxEngine)
    {
        m_RotaxEngine = rotaxEngine;
        m_IsRotaxEngineAttached = true;

        if (GetGame().IsServer())
        {
           // GetGame().RPCSingleParam(this, ExpansionHotAirBalloonRPCEnum.RPC_EXPANSION_HOT_AIR_BALLOON_ATTACH_ROTAX_ENGINE, NULL, true);
        }
    }

    void DetachRotaxEngine()
    {
        m_RotaxEngine = NULL;
        m_IsRotaxEngineAttached = false;

        if (GetGame().IsServer())
        {
          //  GetGame().RPCSingleParam(this, ExpansionHotAirBalloonRPCEnum.RPC_EXPANSION_HOT_AIR_BALLOON_DETACH_ROTAX_ENGINE, NULL, true);
        }
    }
*/
    void SetGasLevel(float gasLevel)
    {
        m_GasLevel = gasLevel;

        if (m_GasLevel < 0)
        {
            m_GasLevel = 0;
        }

        if (m_GasLevel > 1)
        {
            m_GasLevel = 1;
        }

        if (GetGame().IsServer())
        {
          //  GetGame().RPCSingleParam(this, ExpansionHotAirBalloonRPCEnum.RPC_EXPANSION_HOT_AIR_BALLOON_SET_GAS_LEVEL, new Param1<float>(m_GasLevel), true);
        }
    }

    void SetAltitude(float altitude)
    {
        m_Altitude = altitude;

        if (m_Altitude < 0)
        {
            m_Altitude = 0;
        }

        if (GetGame().IsServer())
        {
           // GetGame().RPCSingleParam(this, ExpansionHotAirBalloonRPCEnum.RPC_EXPANSION_HOT_AIR_BALLOON_SET_ALTITUDE, new Param1<float>(m_Altitude), true);
        }
    }

// ----------------- INPUT -----------------------

	void ProcessPlayerInput()
	{
		PlayerBase playerGame = PlayerBase.Cast(GetGame().GetPlayer());

		if (playerGame && playerGame.GetCommand_Vehicle() && playerGame.GetCommand_Vehicle().GetTransport() == this)
		{
			PlayerBase crewMember = PlayerBase.Cast(CrewMember(0));

			if (crewMember == playerGame)
			{
				UAInterface input_source = playerGame.GetInputInterface();

				if (input_source)
				{
					ProcessInputCommands(input_source);
				}
			}
		}
	}

	void ProcessInputCommands(UAInterface input_source)
	{
		float calc_input = input_source.SyncedValue("UAHeatBlasterUp");
		ProcessHeatBlasterUp(calc_input);
		calc_input = input_source.SyncedValue("UAHeatBlasterDown");
		ProcessHeatBlasterDown(calc_input);
		calc_input = input_source.SyncedValue("UARotaxThrottleUp");
		ProcessRotaxUp(calc_input);
		calc_input = input_source.SyncedValue("UARotaxThrottleDown");
		ProcessRotaxDown(calc_input);
		calc_input = input_source.SyncedValue("UARotaxLeftTurn");
		ProcessRotaxLeft(calc_input);
		calc_input = input_source.SyncedValue("UARotaxRightTurn");
		ProcessRotaxRight(calc_input);
		calc_input = input_source.SyncedValue("UATiggleCompressor");
		ProcessToggleCompressor(calc_input);
		calc_input = input_source.SyncedValue("UAToggleGasValve");
		ProcessToggleFuel(calc_input);
		calc_input = input_source.SyncedValue("UAPressIgniter");
		ProcessPressIgniter(calc_input);	
    }

	void ProcessHeatBlasterUp(float calc_input)
	{
		if (calc_input > 0.5)
		{
			if (m_heat_increase_press == 0)
			{
				m_heat_level = Math.Clamp((m_heat_level + 1), 0, 20);
				SetSynchDirty();
			}
			m_heat_increase_press += 1;

			if (m_heat_increase_press > 5)
			{
				m_heat_increase_press = 0;
			}
		}
		else
		{
			m_heat_increase_press = 0;
		}
	}

	void ProcessHeatBlasterDown(float calc_input)
	{
		if (calc_input > 0.5)
		{
			if (m_heat_decrease_press == 0)
			{
				m_heat_level = Math.Clamp((m_heat_level - 1), 0, 20);
				SetSynchDirty();
			}
			m_heat_decrease_press += 1;

			if (m_heat_decrease_press > 5)
			{
				m_heat_decrease_press = 0;
			}
		}
		else
		{
			m_heat_decrease_press = 0;
		}
	}

	void ProcessRotaxUp(float calc_input)
	{
		if (calc_input > 0.5)
		{
			if (m_rotax_increase_press == 0)
			{
                if (m_IsFlying)
                {
                    m_IsForwardFlight = true;
                }

				m_throttle_level = Math.Clamp((m_throttle_level + 1), 0, 20);
				SetSynchDirty();
			}
			m_rotax_increase_press += 1;

			if (m_rotax_increase_press > 5)
			{
				m_rotax_increase_press = 0;
                m_IsForwardFlight = false;
			}
		}
		else
		{
			m_rotax_increase_press = 0;
                m_IsForwardFlight = false;
		}
	}

	void ProcessRotaxDown(float calc_input)
	{
		if (calc_input > 0.5)
		{
			if (m_rotax_decrease_press == 0)
			{
                if (m_IsFlying)
                {
                    m_IsAirbrake = true;
                }

				m_throttle_level = Math.Clamp((m_throttle_level - 1), 0, 20);
				SetSynchDirty();
			}
			m_rotax_decrease_press += 1;

			if (m_rotax_decrease_press > 5)
			{
				m_rotax_decrease_press = 0;
                m_IsAirbrake = false;
			}
		}
		else
		{
			m_rotax_decrease_press = 0;
                m_IsAirbrake = false;
		}
	}

	void ProcessRotaxLeft(float calc_input)
	{
		if (calc_input > 0.5)
		{
			if (m_rotax_left_press == 0)
			{
                if (m_IsRotaxAttached && m_IsFlying)
                {
                    m_IsSteeringLeft = true;
                }
				SetSynchDirty();
			}
			m_rotax_left_press += 1;

			if (m_rotax_left_press > 5)
			{
				m_rotax_left_press = 0;
                m_IsSteeringLeft = false;
			}
		}
		else
		{
			m_rotax_left_press = 0;
            m_IsSteeringLeft = false;
		}
	}

	void ProcessRotaxRight(float calc_input)
	{
		if (calc_input > 0.5)
		{
			if (m_rotax_right_press == 0)
			{
                if (m_IsRotaxAttached && m_IsFlying)
                {
                    m_IsSteeringRight = true;
                }
				SetSynchDirty();
			}
			m_rotax_right_press += 1;

			if (m_rotax_right_press > 5)
			{
				m_rotax_right_press = 0;
                m_IsSteeringRight = false;
			}
		}
		else
		{
			m_rotax_right_press = 0;
            m_IsSteeringRight = false;
		}
	}

	void ProcessToggleCompressor(float calc_input)
	{
		if (calc_input > 0.5)
		{
			if (m_compressor_toggle_press == 0)
			{
                if (m_IsLanded && !m_IsFlying)
                {
                    m_IsLanded = false;
                    m_IsFlying = true;
                    //GetGame().RPCSingleParam(this, ERPCs.RPC_SOUND_BALLOON_AIR_GENERATOR, null, true);
                    //GetGame().RPCSingleParam(this, ERPCs.RPC_SOUND_BALLOON_COMPRESSOR, null, true);
                   // GetGame().RPCSingleParam(this, ERPCs.RPC_ANIMATION_BALLOON_LOW, null, true);
                }
			}
			m_compressor_toggle_press += 1;
			if (m_compressor_toggle_press > 5)
			{
				m_compressor_toggle_press = 0;
            	//GetGame().RPCSingleParam(this, ERPCs.RPC_SOUND_BALLOON_AIR_GENERATOR, null, false);
			}
		}
		else
		{
			m_compressor_toggle_press = 0;
            //GetGame().RPCSingleParam(this, ERPCs.RPC_SOUND_BALLOON_AIR_GENERATOR, null, false);
		}
		SetSynchDirty();
	}

	void ProcessToggleFuel(float calc_input)
	{
		if (calc_input > 0.5)
		{
			if (m_gas_switch_toggle_press == 0)
			{
                if (m_IsGasOn && m_GasLevel > MINIMUM_GAS_LEVEL)
                {
                    m_IsBurnerOn = !m_IsBurnerOn;
                    if (m_IsBurnerOn)
                    {
                     //   GetGame().RPCSingleParam(this, ERPCs.RPC_PARTICLE_BALLOON_BURNER_FIRE, null, true);
                    }
                }
			}
			m_gas_switch_toggle_press += 1;
			if (m_gas_switch_toggle_press > 5)
			{
				m_gas_switch_toggle_press = 0;
            	//GetGame().RPCSingleParam(this, ERPCs.RPC_SOUND_BALLOON_AIR_GENERATOR, null, false);
			}
		}
		else
		{
			m_gas_switch_toggle_press = 0;
           // GetGame().RPCSingleParam(this, ERPCs.RPC_SOUND_BALLOON_AIR_GENERATOR, null, false);
		}
		SetSynchDirty();
	}

	void ProcessPressIgniter(float calc_input)
	{
		if (calc_input > 0.5)
		{
			if (m_IsGasOn && m_GasLevel > MINIMUM_GAS_LEVEL)
			{
				if(!m_IsBurnerOn)
				{
	                if (m_IsRotaxAttached && !m_IsRotaxOn)
	                {
	                    m_IsRotaxOn = true;
	                 //   GetGame().RPCSingleParam(this, ERPCs.RPC_SOUND_BALLOON_ROTAX_START, null, true);
	                }
				}
				
				m_IsBurnerOn = !m_IsBurnerOn;
				
				if (m_IsBurnerOn)
				{
					//GetGame().RPCSingleParam(this, ERPCs.RPC_SOUND_BALLOON_BURNER_IGNITE, null, true);
					//GetGame().RPCSingleParam(this, ERPCs.RPC_PARTICLE_BALLOON_BURNER_FIRE, null, true);
				}
			}
			m_igniter_press += 1;
			if (m_igniter_press > 5)
			{
				m_igniter_press = 0;
            	//GetGame().RPCSingleParam(this, ERPCs.RPC_SOUND_BALLOON_BURNER_IGNITE, null, false);
            	//GetGame().RPCSingleParam(this, ERPCs.RPC_PARTICLE_BALLOON_BURNER_FIRE, null, false);
			}
		}
		else
		{
			m_igniter_press = 0;
           // GetGame().RPCSingleParam(this, ERPCs.RPC_SOUND_BALLOON_BURNER_IGNITE, null, false);
            //GetGame().RPCSingleParam(this, ERPCs.RPC_PARTICLE_BALLOON_BURNER_FIRE, null, false);
		}
		SetSynchDirty();
	}

 // ---------------------- MATH ---------------------------

    void SetWindDirection(float windDirection)
    {
        m_WindDirection = windDirection;
    }

    void SetWindSpeed(float windSpeed)
    {
        m_WindSpeed = windSpeed;
    }

    // Implement a function that calculates the altitude gain of the balloon based on the gas level variable
    float CalculateAltitudeGain(float gasLevel, float weight, float windSpeed, float windDirection)
    {
        float altitudeGain = 0.0;

        if (gasLevel > MINIMUM_GAS_LEVEL)
        {
            float gasPower = gasLevel / MAXIMUM_GAS_LEVEL;
            float weightPower = weight / MAXIMUM_WEIGHT;
            float windPower = windSpeed / MAXIMUM_WIND_SPEED;
            float windDirectionPower = (windDirection / WIND_DIRECTION_MAX) * 2 * Math.PI;

            float upwardForce = gasPower * weightPower;
            float horizontalForce = windPower * Math.Cos(windDirectionPower);
            float verticalForce = windPower * Math.Sin(windDirectionPower);

            altitudeGain = upwardForce - verticalForce;
        }

        return altitudeGain;
    }

    // Consider wind conditions as a crucial factor in determining the flight path of the balloon
    void CalculateWindEffect(float windSpeed, float windDirection)
    {
        if (m_IsFlying)
        {
            float windPower = windSpeed / MAXIMUM_WIND_SPEED;
            float windDirectionPower = (windDirection / WIND_DIRECTION_MAX) * 2 * Math.PI;

            float horizontalForce = windPower * Math.Cos(windDirectionPower);
            float verticalForce = windPower * Math.Sin(windDirectionPower);

            m_Altitude += verticalForce;
            m_WindDirection += horizontalForce;
        }
    }

// ------------------- TASK FUNCTIONS --------------------


   private void HandleGasLevel()
    {
        // Handle gas level

        if (m_IsGasOn && m_GasLevel < MAXIMUM_GAS_LEVEL)
        {
            m_GasLevel += 0.01;
        }
        else if (!m_IsGasOn && m_GasLevel > MINIMUM_GAS_LEVEL)
        {
            m_GasLevel -= 0.01;
        }
    }

    private void HandleAltitude()
    {
        // Handle altitude
        
        if (m_GasLevel > MINIMUM_GAS_LEVEL && m_Altitude < MAXIMUM_ALTITUDE)
        {
            m_Altitude += m_GasLevel * 10.0;
        }
        else if (m_Altitude > MINIMUM_ALTITUDE)
        {
            m_Altitude -= 1.0;
        }
    }

    void InflateBalloon()
    {
        if (m_Altitude < MAXIMUM_ALTITUDE)
        {
            float altitudeGain = CalculateAltitudeGain(m_GasLevel, m_Weight, m_WindSpeed, m_WindDirection);
            m_Altitude += altitudeGain;

            if (m_Altitude > MAXIMUM_ALTITUDE)
            {
                m_Altitude = MAXIMUM_ALTITUDE;
            }
        }

        if (m_Altitude >= MAXIMUM_ALTITUDE && m_IsFlying)
        {
         //   PlaySound(ERPCs.RPC_SOUND_BALLOON_INFLATE);
         //   GetGame().RPCSingleParam(this, ERPCs.RPC_ANIMATION_BALLOON_LOW_HIDE, null, true);
        //    GetGame().RPCSingleParam(this, ERPCs.RPC_ANIMATION_BALLOON_HALF_SHOW, null, true);
        }
    }


	static void DisablePhysics(Object ent)
	{
		SetVelocity( ent, Vector( 0, 0, 0 ) );
		dBodySetAngularVelocity( ent, Vector( 0, 0, 0 ) );
		dBodyActive( ent, ActiveState.INACTIVE );
		dBodyDynamic( ent, false );
	}

	static void EnablePhysics(Object ent)
	{
		SetVelocity( ent, Vector( 0, 0, 0 ) );
		dBodySetAngularVelocity( ent, Vector( 0, 0, 0 ) );
		dBodyActive( ent, ActiveState.ALWAYS_ACTIVE );
		dBodyDynamic( ent, true );
	}

// ---------------- PRE - UPDATE -----------------------

    override void OnVariablesSynchronized()
    {
        super.OnVariablesSynchronized();

        string attachedItem;
/*
        if (GetGame().IsServer() && !GetGame().IsMultiplayer()) 
        {
            if (m_Vehicle.GetInventory().FindAttachment(ATTACHMENT_NAME) != NULL) 
            {
                attachedItem = ATTACHMENT_NAME;
            }
        }
	*/

        if (m_IsCompressorOn)
        {
            if (!m_CompressorSound.IsSoundPlaying())
            {
                m_CompressorSound.SetSoundAutodestroy(false);
                m_CompressorSound.SetSoundLoop(true);
                m_CompressorSound.Play();
            }
        }
        else
        {
            if (m_CompressorSound.IsSoundPlaying())
            {
                m_CompressorSound.Stop();
            }
        }

        if (m_IsGasOn)
        {
            if (!m_GasFlowSound.IsSoundPlaying())
            {
                m_GasFlowSound.SetSoundAutodestroy(false);
                m_GasFlowSound.SetSoundLoop(true);
                m_GasFlowSound.Play();
            }
        }
        else
        {
            if (m_GasFlowSound.IsSoundPlaying())
            {
                m_GasFlowSound.Stop();
            }
        }

        if (m_IsBurnerOn)
        {
            if (!m_IgniterSound.IsSoundPlaying())
            {
                m_IgniterSound.SetSoundAutodestroy(false);
                m_IgniterSound.SetSoundLoop(false);
                m_IgniterSound.Play();
            }

            if (m_FireParticles)
            {
                m_FireParticles.Play();
            }
        }
        else
        {
            if (m_IgniterSound.IsSoundPlaying())
            {
                m_IgniterSound.Stop();
            }

            if (m_FireParticles)
            {
                m_FireParticles.Stop();
            }
        }

        if (m_IsLanding)
        {
            if (GetAnimationPhase("source_balloonfull") > 0)
            {
                SetAnimationPhase("source_balloonfull", GetAnimationPhase("source_balloonfull") - 0.01);
            }

            if (GetAnimationPhase("source_hideballoonfull") < 1)
            {
                SetAnimationPhase("source_hideballoonfull", GetAnimationPhase("source_hideballoonfull") + 0.01);
            }

            if (GetAnimationPhase("source_balloonhalf") > 0)
            {
                SetAnimationPhase("source_balloonhalf", GetAnimationPhase("source_balloonhalf") - 0.01);
            }

            if (GetAnimationPhase("source_hideballoonhalf") < 1)
            {
                SetAnimationPhase("source_hideballoonhalf", GetAnimationPhase("source_hideballoonhalf") + 0.01);
            }

            if (GetAnimationPhase("source_balloonempty") > 0)
            {
                SetAnimationPhase("source_balloonempty", GetAnimationPhase("source_balloonempty") - 0.01);
            }

            if (GetAnimationPhase("source_hideballoonempty") < 1)
            {
                SetAnimationPhase("source_hideballoonempty", GetAnimationPhase("source_hideballoonempty") + 0.01);
            }

            if (m_CompressorSound.IsSoundPlaying())
            {
                m_CompressorSound.Stop();
            }

            if (m_GasFlowSound.IsSoundPlaying())
            {
                m_GasFlowSound.Stop();
            }

            if (m_IgniterSound.IsSoundPlaying())
            {
                m_IgniterSound.Stop();
            }

            if (m_LowGasSound.IsSoundPlaying())
            {
                m_LowGasSound.Stop();
            }

            if (m_GasBlowOffSound.IsSoundPlaying())
            {
                m_GasBlowOffSound.Stop();
            }
        }
    }

    void UpdateRotaxInput(float deltaTime)
    {
        if (m_IsRotaxEngineAttached)
        {
            if (GetGame().IsServer())
            {
                if (m_RotaxEngine)
                {
                    float rotaxEngineSpeed = 0;

                    if (GetGame().IsMultiplayer() && GetGame().IsServer())
                    {
                        rotaxEngineSpeed = GetRPCFloat();
                    }
                    else
                    {
                        if (GetGame().GetInput().LocalValue("UAExpansionUAHotAirBalloonEngineForward"))
                        {
                            rotaxEngineSpeed += 0.1;
                        }

                        if (GetGame().GetInput().LocalValue("UAExpansionUAHotAirBalloonEngineBackward"))
                        {
                            rotaxEngineSpeed -= 0.1;
                        }

                        if (rotaxEngineSpeed > 1)
                        {
                            rotaxEngineSpeed = 1;
                        }

                        if (rotaxEngineSpeed < -1)
                        {
                            rotaxEngineSpeed = -1;
                        }

                        SetRPCFloat(rotaxEngineSpeed);
                    }

                    m_RotaxEngine.SetAnimationPhase("rotax_engine_propeller", rotaxEngineSpeed);
                }
            }
        }
    }
 
 // ------------------ ON UPDATE -----------------------

   override void OnUpdate(float deltaTime)
    {
        super.OnUpdate(deltaTime);

        HandleGasLevel(); 
        HandleAltitude(); 
        HandleWind(); 
        HandleAnimations(); 
        HandleSounds();

        UpdateRotaxInput(deltaTime)

        m_IsLanded = IsLanded();
        m_IsPlayerInside = IsPlayerInside();
        m_IsDisconnecting = IsDisconnecting();

        if (m_IsDisconnecting || !m_IsPlayerInside || m_IsLanded)
        {
            StopSounds();
            ResetVariables();
            return;
        }
    }

 // ---------------------- ON SIMULATION -----------------------
 
    override void EOnSimulate(IEntity owner, float dt)
    {
		MapFallFix();

		ProcessPlayerInput();
    }

 // ------------------ ON - POSTSIMULATION -----------------------

    override void EOnPostSimulate(IEntity other, float timeSlice)
    {
        m_Time += timeSlice;

        if ((GetGame().IsClient() || !GetGame().IsMultiplayer()))
        {

		}
    }

// ------------------------ MAP - FIX ------------------------------

    void MapFallFix()
    {
        if (GetGame().IsServer())
        {
            vector calc_breakthrough = GetPosition();
            vector calc_ground = calc_breakthrough;
            calc_ground[1] = GetGame().SurfaceY(calc_ground[0], calc_ground[2]);
            float calc_undermap = vector.Distance(calc_breakthrough, calc_ground);
            if (calc_breakthrough[1] < (calc_ground[1] - 1.75))
            {
                vector calc_PtcPosition = GetPosition();
                calc_PtcPosition[1] = calc_PtcPosition[1] + calc_undermap;
                SetPosition(calc_PtcPosition);
            }

            if (MemoryPointExists("map_break_front"))
            {
                vector nose_point = GetMemoryPointPos("map_break_front");
                vector nose_world = ModelToWorld(nose_point);
                vector nose_ground = nose_world;
                nose_ground[1] = GetGame().SurfaceY(nose_ground[0], nose_ground[2]);
                calc_undermap = vector.Distance(nose_world, nose_ground);
                if (nose_world[1] < (nose_ground[1] - 0.20))
                {
                    calc_PtcPosition = GetPosition();
                    calc_PtcPosition[1] = calc_PtcPosition[1] + calc_undermap;
                    SetPosition(calc_PtcPosition);
                }
            }

            if (MemoryPointExists("map_break_left"))
            {
                vector left_tail_point = GetMemoryPointPos("map_break_left");
                vector left_tail_world = ModelToWorld(left_tail_point);
                vector left_tail_ground = left_tail_world;
                left_tail_ground[1] = GetGame().SurfaceY(left_tail_ground[0], left_tail_ground[2]);
                calc_undermap = vector.Distance(left_tail_world, left_tail_ground);
                if (left_tail_world[1] < (left_tail_ground[1] - 0.20))
                {
                    calc_PtcPosition = GetPosition();
                    calc_PtcPosition[1] = calc_PtcPosition[1] + calc_undermap;
                    SetPosition(calc_PtcPosition);
                }
            }

            if (MemoryPointExists("map_break_right"))
            {
                vector right_tail_point = GetMemoryPointPos("map_break_right");
                vector right_tail_world = ModelToWorld(right_tail_point);
                vector right_tail_ground = right_tail_world;
                right_tail_ground[1] = GetGame().SurfaceY(right_tail_ground[0], right_tail_ground[2]);
                calc_undermap = vector.Distance(right_tail_world, right_tail_ground);
                if (right_tail_world[1] < (right_tail_ground[1] - 0.20))
                {
                    calc_PtcPosition = GetPosition();
                    calc_PtcPosition[1] = calc_PtcPosition[1] + calc_undermap;
                    SetPosition(calc_PtcPosition);
                }
            }
        }
    }

 // ------------------ ENGINE STARTED -----------------------

    override void OnEngineStart()
    {
        super.OnEngineStart();

        if(IsBalloon())
        {
            m_BlasterSoundFX = true;

			if ((GetGame().IsClient() || !GetGame().IsMultiplayer()))
			{
				if (EngineIsOn())
				{
					if (!SEffectManager.IsEffectExist(m_blaster1PtcFx) || !SEffectManager.IsEffectExist(m_blaster2PtcFx))
					{
						m_blaster1Fx = new EffExhaustSmoke();
						m_blaster1PtcFx = SEffectManager.PlayOnObject(m_blaster1Fx, this, m_blaster1PtcPos, m_blaster1PtcDir);
						m_blaster1Fx.SetParticleStateLight();
						m_blaster2Fx = new EffExhaustSmoke();
						m_blaster2PtcFx = SEffectManager.PlayOnObject(m_blaster2Fx, this, m_blaster2PtcPos, m_blaster2PtcDir);
						m_blaster2Fx.SetParticleStateLight();
					}
				}
			}
		}
        
    }

 // ------------------ ENGINE OFF -----------------------

    override void OnEngineStop()
    {
        super.OnEngineStop();

        if(IsBalloon())
        {
            m_heat_level = 0;
            m_BlasterSoundFX = false;

            if ((GetGame().IsClient() || !GetGame().IsMultiplayer()))
            {
				if (m_blaster1PtcFx && SEffectManager.IsEffectExist(m_blaster1PtcFx)) 
					SEffectManager.Stop(m_blaster1PtcFx);
				if (m_blaster2PtcFx && SEffectManager.IsEffectExist(m_blaster2PtcFx)) 
					SEffectManager.Stop(m_blaster2PtcFx);
            }
			
            if (m_IsRotaxAttached && m_IsRotaxOn)
            {
                m_IsRotaxOn = false;
                GetGame().RPCSingleParam(this, ERPCs.RPC_SOUND_BALLOON_ROTAX_STOP, null, true);
            }
        }
    }

 // ------------------ ON - TICK -----------------------

    void OnTick()
    {
        super.OnTick();

        if (GetGame().IsServer())
        {
            m_Weather = GetGame().GetWeather();
            m_WindDirection = m_Weather.GetWindDirection();
            m_WindSpeed = m_Weather.GetWindSpeed();
        }
    }

 // ------------------ ON - CONTACT -----------------------




 // ------------------ ON - STARTS -----------------------

    override void EOnInit(IEntity other, int extra)
    {   
        m_FireEffect = new Effect;
        SetNeededParts();
    }

	override void EEInit()
	{
		super.EEInit();

        m_GasLevel = 0.0;
        m_Altitude = 0.0;
        m_WindSpeed = 0.0;
        m_WindDirection = "0 0 0";
        m_IsPlayerInside = false;
        m_IsGasOn = false;
        m_IsBurnerOn = false;
        m_IsLanded = true;
        m_IsDisconnecting = false;

        m_CompressorSound = m_SoundBuilder.BuildSoundObject();
        m_GasFlowSound = m_SoundBuilder.BuildSoundObject();
        m_IgniterSound = m_SoundBuilder.BuildSoundObject();
        m_LowGasSound = m_SoundBuilder.BuildSoundObject();
        m_GasBlowOffSound = m_SoundBuilder.BuildSoundObject();
        m_InFlightSound = m_SoundBuilder.BuildSoundObject();
        m_WindSound = m_SoundBuilder.BuildSoundObject();
        m_BurnerFireSound = m_SoundBuilder.BuildSoundObject();

        GetGame().RPCSingleParam(this, RPC_INIT_HOT_AIR_BALLOON, null, true);

		if ( GetGame().IsServer())
		{
            m_dmgContactCoef = 0.001;

            m_HotAirBalloonConfigData = new Hot_Air_Balloon_Config_Data("Balloon_Base", FuelCap, FuelRate, AirspeedMax, AltitudeMax, ClimbMax, AeroDrag, Blaster_Heat_Rate);
            LoadBalloonConfiguration("Balloon_Base", m_HotAirBalloonConfigData);
		}
	}

    void SetNeededParts()
    {
        GetInventory().CreateAttachment("smalldummywheel");
        GetInventory().CreateAttachment("smalldummywheel");
        GetInventory().CreateAttachment("smalldummywheel");
        GetInventory().CreateAttachment("smalldummywheel");
        GetInventory().CreateAttachment("CarBattery");
        GetInventory().CreateAttachment("SparkPlug");
        GetInventory().CreateAttachment("CarRadiator");
        SetSynchDirty();
    }

 // --------------------- LIGHTINGS -----------------------

    // !! TODO: ADD LIGHTING CONFIG AT SOME POINT !!

// ---------------------- SOUNDS -----------------------

    override float OnSound(CarSoundCtrl ctrl, float oldValue)
    {
        switch (ctrl)
        {
        case CarSoundCtrl.DOORS:
            float newValue = 0;

            if (GetCarDoorsState("Balloon_Base_doors_codriver") == CarDoorState.DOORS_CLOSED)
                newValue += 0.25;
            if (newValue > 1)
                newValue = 1;
            return newValue;
            break;
        }

        return oldValue;
    }
















 // ---------------- CREW - ATTACHMENTS - CARGO ----------------

    override bool CanDisplayCargo()
    {
        if (!super.CanDisplayCargo())
            return false;

        return true;
    }

    override bool CanReachDoorsFromSeat(string pDoorsSelection, int pCurrentSeat)
    {
        return true;
    }

    override bool CanReleaseAttachment(EntityAI attachment)
    {
        if (!super.CanReleaseAttachment(attachment))
            return false;

        string attType = attachment.GetType();

        if (attType.Contains("Balloon_Base"))
            return false;

        return true;
    }

    override bool CanDisplayAttachmentCategory(string category_name)
    {
        return true;
    }

    override int GetCarDoorsState(string slotType)
    {
        return CarDoorState.DOORS_MISSING;
    }

    override bool CrewCanGetThrough(int posIdx)
    {
        return true;
    }

    override string GetDoorSelectionNameFromSeatPos(int posIdx)
    {
        switch (posIdx)
        {
        case 0:
            return "doors_driver";
            break;
        case 1:
            return "doors_codriver";
            break;
        case 2:
            return "doors_cargo1";
            break;
        case 3:
            return "doors_cargo2";
            break;
        }

        return super.GetDoorSelectionNameFromSeatPos(posIdx);
    }

    override int GetSeatIndexFromDoor(string pDoorSelection)
    {
        switch (pDoorSelection)
        {
        case "DoorsDriver":
            return 0;
            break;

        case "DoorsCoDriver":
            return 1;
            break;
        }

        return -1;
    }

    override string GetDoorInvSlotNameFromSeatPos(int posIdx)
    {
        switch (posIdx)
        {
        case 0:
            return "Balloon_Base_doors_driver";
            break;
        case 1:
            return "Balloon_Base_doors_codriver";
            break;
        case 2:
            return "Balloon_Base_doors_cargo1";
            break;

        case 3:
            return "Balloon_Base_doors_cargo2";
            break;
        }

        return super.GetDoorInvSlotNameFromSeatPos(posIdx);
    }

    override string GetAnimSourceFromSelection(string selection)
    {
        switch (selection)
        {
        case "doors_driver":
            return "DoorsDriver";
        case "doors_codriver":
            return "DoorsCoDriver";
        case "doors_cargo1":
            return "DoorsCargo1";
        case "doors_cargo2":
            return "DoorsCargo2";
        }

        return "";
    }

    override int GetCrewIndex(string selection)
    {
        switch (selection)
        {
        case "seat_driver":
            return 0;

        case "seat_codriver":
            return 1;

        case "seat_cargo1":
            return 2;

        case "seat_cargo2":
            return 3;

        case "seat_cargo3":
            return 4;
        }

        return -1;
    }

    override string GetDoorConditionPointFromSelection(string selection)
    {
        switch (selection)
        {
        case "seat_driver":
            return "seat_con_1_1";
            break;

        case "seat_codriver":
            return "seat_con_2_1";
            break;

        case "seat_cargo1":
            return "seat_con_1_2";
            break;

        case "seat_cargo2":
            return "seat_con_2_2";
            break;

        case "seat_cargo3":
            return "seat_con_2_3";
            break;
        }

        return "";
    }

    override int GetSeatAnimationType(int posIdx)
    {
        switch (posIdx)
        {
        case 0:
            return DayZPlayerConstants.VEHICLESEAT_DRIVER;
        case 1:
            return DayZPlayerConstants.VEHICLESEAT_CODRIVER;
        case 2:
            return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
        case 3:
            return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
        case 4:
            return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
        case 5:
            return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
        case 6:
            return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
        case 7:
            return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
        case 8:
            return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
        case 9:
            return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
        case 10:
            return DayZPlayerConstants.VEHICLESEAT_PASSENGER_L;
        case 11:
            return DayZPlayerConstants.VEHICLESEAT_PASSENGER_R;
        }
        return 0;
    }

// -------------------- VITALS ------------------------

    override float GetTransportCameraDistance()
    {
        return 28.0;
    }

    override bool IsVitalCarBattery()
    {
        return false;
    }

    override bool IsVitalGlowPlug()
    {
        return false;
    }

    override bool IsVitalSparkPlug()
    {
        return false;
    }

    override bool IsVitalRadiator()
    {
        return false;
    }

    override bool IsVitalTruckBattery()
    {
        return false;
    }

    override bool IsVitalEngineBelt()
    {
        return false;
    }

    override bool IsVitalFuelTank()
    {
        return false;
    }

// ------------------------ DEBUG ---------------------------

    void CheckPlayerDisconnect()
    {
        if (!GetGame().IsMultiplayer() || GetGame().IsClient() || GetGame().IsServer())
        {
            return;
        }
        
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        
        if (!player)
        {
            BringBalloonToGround();
        }
    }

    void BringBalloonToGround()
    {
        while (m_Altitude > 0)
        {
            m_Altitude -= 1;
            SetPosition(GetPosition() + Vector(0, -1, 0));
            Sleep(1000);
        }
        StopAllSounds();
    }

		/*
    override void OnDisconnect()
    {
        super.OnDisconnect();
        
        BringBalloonToGround();
    }
	*/

    override void OnStoreSave(ParamsWriteContext ctx)
    {
        // Save the gas level
        ctx.Write(m_GasLevel);

        // Save the position
        vector position = GetPosition();
        ctx.Write(position[0]);
        ctx.Write(position[2]);

        // Save any attached objects
        if (m_AttachedObject)
        {
            ctx.Write(true);
            ctx.Write(m_AttachedObject.GetType());
            m_AttachedObject.OnStoreSave(ctx);
        }
        else
        {
            ctx.Write(false);
        }
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        // Load the gas level
        if (!ctx.Read(m_GasLevel))
            return false;

        // Load the position
        float x, z;

        if (!ctx.Read(x) || !ctx.Read(z))
            return false;

        SetPosition(Vector(x, 0, z));

        // Load any attached objects
        bool hasAttachedObject;

        if (!ctx.Read(hasAttachedObject))
            return false;

        if (hasAttachedObject)
        {
            string objectType;
            if (!ctx.Read(objectType))
                return false;

            m_AttachedObject = ItemBase.Cast(GetGame().CreateObject(objectType, "0 0 0"));

            if (!m_AttachedObject)
                return false;

            if (!m_AttachedObject.OnStoreLoad(ctx, version))
                return false;
        }
        else
        {
            m_AttachedObject = NULL;
        }

        return true;
    }

    override void OnDebugSpawn()
    {
        EntityAI entity;

        if (Class.CastTo(entity, this))
        {
            entity.GetInventory().CreateAttachment("CarBattery");
            entity.GetInventory().CreateAttachment("CarRadiator");
            entity.GetInventory().CreateAttachment("SparkPlug");
            entity.GetInventory().CreateAttachment("smalldummywheel");
            entity.GetInventory().CreateAttachment("smalldummywheel");
            entity.GetInventory().CreateAttachment("smalldummywheel");
            entity.GetInventory().CreateAttachment("smalldummywheel");
        };

        Fill(CarFluid.FUEL, 130.0);
        Fill(CarFluid.OIL, 4.0);
    }
};

// ------------------- TESTING - DAMAGE PARTS --------------------
/*
class HotAirBalloonDAM_Base: Balloon_Base
{
    private float fuelLevel;
    private float gasLevel;
    private float balloonCondition;
    private float timer;
    private float burnerDamage;
    private float envelopeDamage;
    private float basketDamage;
    private float fuelTankDamage;
    private float gasTankDamage;

    void HotAirBalloonMaintenance()
    {
        fuelLevel = 100;
        gasLevel = 100;
        balloonCondition = 100;
        timer = 0;
        burnerDamage = 0;
        envelopeDamage = 0;
        basketDamage = 0;
        fuelTankDamage = 0;
        gasTankDamage = 0;
    }

   void RepairComponent(string component)
    {
        if (component == "burner") burnerDamage = 0;
        else if (component == "envelope") envelopeDamage = 0;
        else if (component == "basket") basketDamage = 0;
        else if (component == "fuel tank") fuelTankDamage = 0;
        else if (component == "gas tank") gasTankDamage = 0;
    }

    void CheckCondition()
    {
        if (burnerDamage > 50 || envelopeDamage > 50 || basketDamage > 50 || fuelTankDamage > 50 || gasTankDamage > 50)
        {
            NotificationSystem.Notify("Hot air balloon needs repairs.");
        }

        else if (timer > 1000)
        {
            NotificationSystem.Notify("Hot air balloon needs maintenance.");
        }
   }

    void ReplaceComponent(string component)
    {
        if (component == "burner")
        {
            burnerDamage = 0;
            Inventory.Consume("burner repair kit", 1);
        }
        else if (component == "envelope")
        {
            envelopeDamage = 0;
            Inventory.Consume("envelope repair kit", 1);
        }
        else if (component == "basket")
        {
            basketDamage = 0;
            Inventory.Consume("basket repair kit", 1);
        }
        else if (component == "fuel tank")
        {
            fuelTankDamage = 0;
            Inventory.Consume("fuel tank repair kit", 1);
        }
        else if (component == "gas tank")
        {
            gasTankDamage = 0;
            Inventory.Consume("gas tank repair kit", 1);
        }
    }

    override void OnUpdate(float deltaTime)
    {
        timer += deltaTime;

        if (timer > 1000)
        {
            burnerDamage += 10;
            envelopeDamage += 5;
            basketDamage += 2;
            fuelTankDamage += 1;
            gasTankDamage += 1;
            timer = 0;
        }

        CheckCondition();

        if (GetGame().IsServer())
        {
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckPlayerDisconnect, 5000, true);
        }
    }
};
*/
// ------------------- INHERITED --------------------

class HotAirBalloon: Balloon_Base{};