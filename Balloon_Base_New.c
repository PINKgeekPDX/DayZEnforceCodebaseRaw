class Balloon_Base extends CarScript
{
    bool m_IsError;
    bool m_IsBalloon = true;
    bool m_IsInputDebug;
    bool m_IsFlightDebug;
    bool m_IsDamageDebug;
    bool m_IsPlayerInside;

    bool m_IsLanded;
    bool m_IsFlying;
	bool m_IsGrounded;

    float m_GasTimer;
    float m_GasLevel;
    bool m_IsGasOn;
    bool m_IsIgniting;
    bool m_IsBurnerOn;
    bool m_IsCompressorOn;

    bool m_IsInflating;
    bool m_IsInflated;
    bool m_IsInflationComplete;

    bool m_IsDeflating;
    bool m_IsDeflated;
    bool m_IsDeflationComplete;

    bool m_IsDescending;
    bool m_IsAscending;
    float m_BalloonMass = dBodyGetMass(this);
    vector m_BallonWorldPosition;

    // rotax addon
    float m_FuelTimer;
    float m_ThrottleLevel
    bool m_IsRotaxOn;
    bool m_IsRotaxAttached;
    bool m_IsForwardFlight;
    bool m_IsSteeringLeft;
    bool m_IsSteeringRight
    bool m_CollisionDamage; 

    ref Effect m_FireEffect, m_FireEffect2;
    ref EffVehicleSmoke m_blaster1Fx, m_blaster2Fx;
    int m_blaster1PtcFx, m_blaster2PtcFx;
    vector m_blaster1PtcPos, m_blaster2PtcPos;
    vector m_blaster1PtcDir, m_blaster2PtcDir;

    float SoundTimer;
    ref SoundObject m_CompressorSound;
    ref SoundObject m_GasFlowSound;
    ref SoundObject m_IgniterSound;
    ref SoundObject m_GasBlowOffSound;
    ref SoundObject m_InFlightSound;
    ref SoundObject m_WindSound;
    ref SoundObject m_BurnerFireSound;
    AbstractWave m_CompressorSoundWave;
    AbstractWave m_GasFlowSoundWave;
    AbstractWave m_IgniterSoundWave;
    AbstractWave m_GasBlowOffSoundWave;
    AbstractWave m_InFlightSoundWave;
    AbstractWave m_WindSoundWave;
    AbstractWave m_BurnerFireSoundWave;
    const string COMPRESSOR_SOUNDSET = "Balloon_Compressor_SoundSet";
    const string GASFLOW_SOUNDSET = "Balloon_GasLow_SoundSet";
    const string IGNITER_SOUNDSET = "Balloon_Igniter_SoundSet";
    const string GASBLOWOFF_SOUNDSET = "Balloon_GasMed_SoundShader";
    const string INFLIGHT_SOUNDSET = "Balloon_AmbFlight_SoundSet";
    const string WIND_SOUNDSET = "Balloon_MainWind_SoundSet";
    const string BURNER_SOUNDSET = "Balloon_GasHigh_SoundShader";
    int m_CompressorSoundPlay = 0;
    int m_GasFlowSoundPlay = 0;
    int m_IgniterSoundPlay = 0;
    int m_GasBlowOffSoundPlay = 0;
    int m_InFlightSoundPlay = 0;
    int m_WindSoundPlay = 0;
    int m_BurnerFireSoundPlay = 0;

    float m_WindSpeedWorld;
    vector m_WindDirectionWorld;

    int m_BalloonState = 0;    //0= balloon NOT operational, 1 = inflate sequence, 2 = balloon IS operational,  3 = deflate sequence
    int m_gas_level = 0;
    int m_throttle_level = 0;

// --------------------------------

    // input keypress
    int m_igniter_press = 0;
    int m_emergency_shutoff_press = 0;
    int m_gas_switch_toggle_press = 0;
    int m_compressor_toggle_press = 0;
    int m_heat_increase_press = 0;
    int m_heat_decrease_press = 0;

    // rotax
    int m_rotax_increase_press = 0;
    int m_rotax_decrease_press = 0;
    int m_rotax_left_press = 0;
    int m_rotax_right_press = 0;

// -----------------------------------

    // constants

    bool WEAPON_DANAGE = false;
    bool COLLISION_DANAGE = false;    
    float COLLISION_DAMAGE_MULTIPLIER = 0.060;
    float GAS_CONSUMPTION_RATE = 0.050;
    float GAS_MAX_CAPACITY = 100.0;
    float FLIGHT_CEILING = 2000.0;      // max altitude in meters
    float FLIGHT_ASCENT_RATE = 0.02000;
    float FLIGHT_DESCENT_RATE = 0.02000;
    float WIND_INFLUENCE = 0.30;    // how much influence the wind forces play on the balloon, higher value makes the Rotax addon have less control factor over the flight control
    float WIND_INTENSITY_MULTIPLIER = 0.9;     // wind intensity and turbulance at higher altitude
    float AERODYNAMIC_DRAG = 0.20;
    float ROTAX_AIRSPEED_MAX = 50.0;
    float ROTAX_FUEL_CAPACITY = 20.0;
    float ROTAX_FUEL_RATE = 0.020833;
    float ROTAX_FORWARD_THRUST_RATE = 0.20;
    float ROTAX_STEERING_RATE = 0.1;
    float BALLOON_INFLATE_TIME = 40000.0;    // time in miliseconds for inflation process (this is how long the compressor needs to run)
    float BALLOON_DEFLATE_TIME = 80000.0;    // time in miliseconds for delation process

// -----------------------------------
    
    // flight telemetry
    float m_BalloonAltitude; // meters
    float m_BalloonAGLAltitude; // agl meters
    float m_BalloonAirSpeedKPH; // kph
    float m_BalloonAirSpeed; // knotts
    vector m_BalloonOrientation; // agl meters

// -----------------------------------

    void Balloon_Base()
    {
        m_dmgContactCoef = COLLISION_DAMAGE_MULTIPLIER;
        m_CarDoorOpenSound = "offroad_door_open_SoundSet";
        m_CarDoorCloseSound = "offroad_door_close_SoundSet";
        m_CompressorSound = BuildSoundObject(COMPRESSOR_SOUNDSET);
        m_GasFlowSound = BuildSoundObject(GASFLOW_SOUNDSET);
        m_IgniterSound = BuildSoundObject(IGNITER_SOUNDSET);
        m_GasBlowOffSound = BuildSoundObject(GASBLOWOFF_SOUNDSET);
        m_InFlightSound = BuildSoundObject(INFLIGHT_SOUNDSET);
        m_WindSound = BuildSoundObject(WIND_SOUNDSET);
        m_BurnerFireSound = BuildSoundObject(BURNER_SOUNDSET);

        SetEventMask( EntityEvent.CONTACT | EntityEvent.SIMULATE | EntityEvent.POSTSIMULATE | EntityEvent.POSTFRAME );

		RegisterNetSyncVariableBool("m_IsInputDebug");
		RegisterNetSyncVariableBool("m_IsFlightDebug");
		RegisterNetSyncVariableBool("m_IsDamageDebug");

		RegisterNetSyncVariableBool("m_IsBalloon");
		RegisterNetSyncVariableInt("m_BalloonState", 0, 4);
		RegisterNetSyncVariableBool("m_IsPlayerInside");

		RegisterNetSyncVariableFloat("m_BalloonAltitude");
		RegisterNetSyncVariableFloat("m_BalloonAGLAltitude");
		RegisterNetSyncVariableFloat("m_BalloonAirSpeed");

		RegisterNetSyncVariableBool("m_IsLanded");
		RegisterNetSyncVariableBool("m_IsFlying");
		RegisterNetSyncVariableBool("m_IsAscending");
    	RegisterNetSyncVariableBool("m_IsDescending");

		RegisterNetSyncVariableFloat("m_GasLevel");
		RegisterNetSyncVariableInt("m_gas_level");

		RegisterNetSyncVariableFloat("m_ThrottleLevel");
		RegisterNetSyncVariableInt("m_throttle_level");

		RegisterNetSyncVariableBool("m_IsIgniting");
		RegisterNetSyncVariableBool("m_IsGasOn");
		RegisterNetSyncVariableBool("m_IsBurnerOn");
		RegisterNetSyncVariableBool("m_IsCompressorOn");

		RegisterNetSyncVariableBool("m_IsRotaxOn");
		RegisterNetSyncVariableBool("m_IsRotaxAttached");
		RegisterNetSyncVariableBool("m_IsForwardFlight");
		RegisterNetSyncVariableBool("m_IsSteeringLeft");
		RegisterNetSyncVariableBool("m_IsSteeringRight");

    	RegisterNetSyncVariableInt("m_igniter_press");
		RegisterNetSyncVariableInt("m_emergency_shutoff_press");
		RegisterNetSyncVariableInt("m_gas_switch_toggle_press");
		RegisterNetSyncVariableInt("m_compressor_toggle_press");
		RegisterNetSyncVariableInt("m_heat_increase_press");
		RegisterNetSyncVariableInt("m_heat_decrease_press");

		RegisterNetSyncVariableInt("m_CompressorSoundPlay");
		RegisterNetSyncVariableInt("m_GasFlowSoundPlay");
		RegisterNetSyncVariableInt("m_IgniterSoundPlay");
		RegisterNetSyncVariableInt("m_GasBlowOffSoundPlay");
		RegisterNetSyncVariableInt("m_InFlightSoundPlay");
		RegisterNetSyncVariableInt("m_WindSoundPlay");
		RegisterNetSyncVariableInt("m_BurnerFireSoundPlay");
	
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

    void ~Balloon_Base()
    {
        if (m_blaster1PtcFx && SEffectManager.IsEffectExist(m_blaster1PtcFx))
    		SEffectManager.Stop(m_blaster1PtcFx);

        if (m_blaster2PtcFx && SEffectManager.IsEffectExist(m_blaster2PtcFx))
    		SEffectManager.Stop(m_blaster2PtcFx);
    }

    override int GetAnimInstance()
    {
        return VehicleAnimInstances.S120;
    }

// -----------------------------------

    void StartCompressor()
    {
        m_IsCompressorOn = true;

    }

    void StopCompressor()
    {
        m_IsCompressorOn = false;

    }

    void StartGasFlow()
    {
        m_IsGasOn = true;

    }


    void StopGasFlow()
    {
        m_IsGasOn = false;

    }

    void StartGasBurner()
    {
        m_IsBurnerOn = true;

    }

    void StopGasBurner()
    {
        m_IsBurnerOn = false;

    }

    void StartInflation()
    {
        m_IsInflating = true;

    }

    void StartDeflation()
    {
        m_IsInflating = false;

    }

    void HandleIgnite()
    {
        m_IsIgniting = (m_igniter_press != 0);
    }

    void Igniter()
    {
        if (!m_IsIgniting)
        {
            m_IsIgniting = true;
            m_IgniterSoundWave = BuildSoundWaveObject(m_IgniterSoundSoundBuilder, m_IgniterSound, GetPosition(), 0, 1.0, false);
            m_IgniterSoundPlay = 1;
        }
    }

    void EmergencyShutoff()
    {
        m_IsGasOn = false;
        m_IsIgniting = false;
        m_IsBurnerOn = false;
        m_IsCompressorOn = false;
    }

    bool CanToggleInflationOrDeflation()
    {
        return !(m_IsFlying || m_IsInflating || m_IsDeflating || (m_GasLevel <= 0));
    }

    void ToggleInflation() 
    {
        if (!CanToggleInflationOrDeflation() || m_BalloonState != 0) return;

        m_IsInflating = true;
        m_CompressorSoundWave = BuildSoundWaveObject(m_CompressorSoundBuilder, m_CompressorSound, GetPosition(), 0, 1.0, false);
        m_CompressorSoundPlay = 1;
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(OnInflationComplete, BALLOON_INFLATE_TIME);
    }

    void ToggleDeflation() 
    {
        if (!CanToggleInflationOrDeflation() || m_BalloonState == 0) return;

        m_IsDeflating = true;
        m_CompressorSoundWave = BuildSoundWaveObject(m_CompressorSoundBuilder, m_CompressorSound, GetPosition(), 0, 1.0, false);
        m_CompressorSoundPlay = 1;
        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(OnDeflationComplete, BALLOON_DEFLATE_TIME);
    }

    void OnInflationComplete() 
    {
        m_IsInflating = false;
        m_CompressorSoundPlay = 0;
        m_CompressorSoundWave.Stop();
        m_BalloonState = 2; // balloon is now operational
    }

    void OnDeflationComplete() 
    {
        m_IsDeflating = false;
        m_CompressorSoundPlay = 0;
        m_CompressorSoundWave.Stop();
        m_BalloonState = 0; // balloon is no longer operational
    }

// ---------------------------------------

	void ConsumeGas(float tick)		
	{
		if (tick - m_GasTimer > 1000)
		{	
			Leak(CarFluid.FUEL, GAS_CONSUMPTION_RATE);
			m_GasTimer = tick;
		}		
	}

	void ConsumeFuel(float tick)		
	{
		if (tick - m_FuelTimer > 1000)
		{	
			Leak(CarFluid.USER1, ROTAX_FUEL_RATE);
			m_FuelTimer = tick;
		}		
	}

// -----------------------------------

    override void EOnInit(IEntity other, int extra)
    {
        //SetNeededParts();

    }

	override void EEInit()
	{
		super.EEInit();

        m_IsPlayerInside = false;
        m_IsLanded = true;
        m_IsFlying = false;
        m_GasTimer = 0.0;
        m_GasLevel = 0.0;
        m_IsGasOn = false;
        m_IsCompressorOn = false;
        m_IsDescending = false;
        m_IsAscending = false;
        m_IsRotaxOn = false;
        m_IsRotaxAttached = false;
        m_IsForwardFlight = false;
        m_IsSteeringLeft = false;
        m_IsSteeringRight = false;
        m_CollisionDamage = false; 
        m_BalloonAltitude = 0.0;
        m_BalloonAGLAltitude = 0.0;
        m_BalloonAirSpeed = 0.0;

		dBodyActive( this, ActiveState.ACTIVE);
		dBodyApplyImpulse( this, "0 1 0");

		UpdateFlightTelemetry();
	}

// -------------------------------

    SoundObject BuildSoundObject(string TargetSoundSet)
    {
        SoundParams soundParams = new SoundParams(TargetSoundSet);
        SoundObjectBuilder soundObjectBuilder = new SoundObjectBuilder(soundParams);

        if (soundObjectBuilder) 
        {
            SoundObject noiseObject = soundObjectBuilder.BuildSoundObject();
            noiseObject.SetPosition(GetPosition());
            return noiseObject;
        }
        return null;
    }

    AbstractWave BuildSoundWaveObject(SoundObject noiseObject, float start_sound, float volume, bool looping)
    {
        if (noiseObject)	
        {
            AbstractWave soundWave = GetGame().GetSoundScene().Play3D(noiseObject, noiseObject.GetSoundObjectBuilder()); 
            if (soundWave)
            {
                soundWave.SetPosition(GetPosition());			
                soundWave.SetVolume(volume);
                soundWave.Loop(looping);	
                soundWave.SetStartOffset(start_sound);
                return soundWave;
            }
        }
        return null;
    }

// ------------------------------

    void UpdateFlightTelemetry()		
	{
		vector BalPosition = GetPosition();
		m_BalloonAltitude = BalPosition[1];
		m_BalloonAGLAltitude = GetBalloonAGL();
		m_BalloonAirSpeedKPH = (GetBalloonAirspeed() * 60 * 60) / 1000;
		m_BalloonAirSpeed = m_BalloonAirSpeedKPH * 0.539957;
		vector BalOrient = GetOrientation();
		m_BalloonOrientation = GetOrientation();
		SetSynchDirty();
	}

	float GetBalloonAGL()		
	{
		return vector.Distance(GetPosition(), FindSolidSurface(GetPosition()));	
	}

	float GetBalloonAirspeed()		
	{
		vector calc_velocity = GetVelocity(this);
		return(Math.Sqrt(Math.Pow(calc_velocity[0],2) + Math.Pow(calc_velocity[1],2) + Math.Pow(calc_velocity[2],2)));		
	}
	
	vector FindSolidSurface(vector position)
	{
        vector contact_pos;
        vector contact_dir;
		vector endPointPos = position - "0 1000 0";
        int contactComponent;
		DayZPhysics.RaycastRV( position, endPointPos,contact_pos,contact_dir,contactComponent,NULL,NULL,NULL,false,false,ObjIntersectGeom,0);

		if (contactComponent==0)
		{
			contact_pos = position;
			contact_pos[1] = GetGame().SurfaceY(contact_pos[0], contact_pos[2]);
		}

		return contact_pos;
	}

// ------------------------------

    void HandlePlayerInput()
    {
        // Igniter press
        if (m_igniter_press)
        {
            m_IsIgniting = !m_IsIgniting;
            m_igniter_press = 0;
        }

        // Emergency shutoff press
        if (m_emergency_shutoff_press)
        {
            m_IsGasOn = m_IsBurnerOn = m_IsCompressorOn = m_IsRotaxOn = false;
            m_emergency_shutoff_press = 0;
        }

        // Gas switch toggle press
        if (m_gas_switch_toggle_press)
        {
            m_IsGasOn = !m_IsGasOn;
            m_gas_switch_toggle_press = 0;
        }

        // Compressor toggle press
        if (m_compressor_toggle_press)
        {
            m_IsCompressorOn = !m_IsCompressorOn;
            m_compressor_toggle_press = 0;
        }

        // Heat increase/decrease press
        if (m_heat_increase_press)
        {
            m_GasLevel = Math.Clamp(m_GasLevel + GAS_CONSUMPTION_RATE, 0, GAS_MAX_CAPACITY);
            m_heat_increase_press = 0;
        }
        else if (m_heat_decrease_press)
        {
            m_GasLevel = Math.Clamp(m_GasLevel - GAS_CONSUMPTION_RATE, 0, GAS_MAX_CAPACITY);
            m_heat_decrease_press = 0;
        }

        // Rotax increase/decrease press
        if (m_rotax_increase_press)
        {
            m_ThrottleLevel = Math.Clamp(m_ThrottleLevel + ROTAX_FUEL_RATE, 0, ROTAX_FUEL_CAPACITY);
            m_rotax_increase_press = 0;
        }
        else if (m_rotax_decrease_press)
        {
            m_ThrottleLevel = Math.Clamp(m_ThrottleLevel - ROTAX_FUEL_RATE, 0, ROTAX_FUEL_CAPACITY);
            m_rotax_decrease_press = 0;
        }

        // Rotax left/right press
        m_IsSteeringLeft = m_rotax_left_press > 0;
        m_IsSteeringRight = m_rotax_right_press > 0;

        if (m_rotax_left_press) m_rotax_left_press = 0;
        if (m_rotax_right_press) m_rotax_right_press = 0;
    }

    // ------------------------------

    static void TogglePhysics(Object ent, bool enable)
    {
        SetVelocity(ent, Vector(0, 0, 0));
        dBodySetAngularVelocity(ent, Vector(0, 0, 0));
        
        if (enable)
        {
            dBodyActive(ent, ActiveState.ALWAYS_ACTIVE);
            dBodyDynamic(ent, true);
        }
        else
        {
            dBodyActive(ent, ActiveState.INACTIVE);
            dBodyDynamic(ent, false);
        }
    }

    // ------------------------------

    vector GetWindDirection()
    {
        Weather weather = GetGame().GetWeather();
        vector wind = weather.GetWind();
        return wind.Normalized();
    }

    vector GetRandomWindDirection()
    {
        return Vector(Math.RandomFloat(-1, 1), Math.RandomFloat(-1, 1), Math.RandomFloat(-1, 1)).Normalized();
    }

    float GetWindSpeed()
    {
        return GetGame().GetWeather().GetWindSpeed();
    }
    float GetRandomWindSpeed()
    {
        return Math.RandomFloat(0, 10);
    }

    // ------------------------------

    override void OnUpdate(float dt)
    {
        super.OnUpdate(dt);
        
        UpdateFlightTelemetry();
        UpdateBalloonState(dt);

        if (GetGame().IsServer())
        {
            m_WindDirectionWorld = GetWindDirection();
            m_WindSpeedWorld = GetWindSpeed();
        }
    }

    override protected void EOnSimulate(IEntity owner, float dt)
    {
        MapFallFix();	
        HandlePlayerInput();	
        UpdateFlight(dt);

        if (GetGame().IsServer())
        {	
            m_gas_level = this.GetFluidFraction(CarFluid.FUEL);		
        }
    }

    override void EOnPostSimulate(IEntity other, float timeSlice)
    {
        m_Time += timeSlice;
        
        if ((GetGame().IsClient() || !GetGame().IsMultiplayer()))
        {

        }

        UpdateSounds(timeSlice);
    }

    void UpdateFlight(float dt)
    {
        m_WindDirectionWorld = GetWindDirection();
        m_WindSpeedWorld = GetWindSpeed();
        
        vector windForce = m_WindDirectionWorld * m_WindSpeedWorld * WIND_INFLUENCE;
        vector airResistance = -GetVelocity(this) * AERODYNAMIC_DRAG;

        dBodyApplyForce(this, windForce);
        dBodyApplyForce(this, airResistance);

        if (m_IsGasOn && m_GasLevel > 0)
        {
            m_GasLevel -= GAS_CONSUMPTION_RATE * dt;
            vector upForce = Vector(0, FLIGHT_ASCENT_RATE * m_GasLevel, 0);
            dBodyApplyForce(this, upForce);
        }
        else
        {
            vector downForce = Vector(0, -FLIGHT_DESCENT_RATE * m_BalloonMass, 0);
            dBodyApplyForce(this, downForce);
        }

        if (m_IsRotaxOn && m_ThrottleLevel > 0)
        {
            m_ThrottleLevel -= ROTAX_FUEL_RATE * dt;

            if (m_IsForwardFlight)
            {
                vector forwardForce = Vector(ROTAX_FORWARD_THRUST_RATE * m_ThrottleLevel, 0, 0);
                dBodyApplyForce(this, forwardForce);
            }

            if (m_IsSteeringLeft)
            {
                vector leftForce = Vector(0, 0, -ROTAX_STEERING_RATE * m_ThrottleLevel);
                dBodyApplyForce(this, leftForce);
            }

            if (m_IsSteeringRight)
            {
                vector rightForce = Vector(0, 0, ROTAX_STEERING_RATE * m_ThrottleLevel);
                dBodyApplyForce(this, rightForce);
            }
        }
    }

    // -----------------------------

    void UpdateBalloonState(float dt)
    {
        switch (m_BalloonState)
        {
            case 0: // Balloon NOT operational
                if (m_IsBalloon && m_IsInflated)
                {
                    m_BalloonState = 2;
                }
                break;

            case 1: // Inflate sequence
                if (m_IsInflationComplete)
                {
                    m_BalloonState = 2;
                }
                break;

            case 2: // Balloon IS operational
                if (!m_IsBalloon || !m_IsInflated)
                {
                    m_BalloonState = 3;
                }
                else
                {
                    if (!m_IsGasOn) StartGasFlow();
                    if (!m_IsBurnerOn) StartGasBurner();
                    // if (!m_IsRotaxAttached) AttachRotaxAddon();
                }
                break;

            case 3: // Deflate sequence
                if (m_IsDeflationComplete)
                {
                    m_BalloonState = 0;
                }
                break;

            default:
                // Handle any unexpected cases or log an error if needed.
                break;
        }
    }

    void UpdateSoundEffects()
    {
        if (m_BalloonAGLAltitude > 20 && m_BalloonState == 2)
        {
            PlayWindSound();
        }

        if (m_heat_increase_press == 1 && m_BalloonState == 2)
        {
            PlayBurnerFireSound();
        }

        if (m_igniter_press == 1)
        {
            PlayIgniterSound();
        }

        if (m_emergency_shutoff_press == 1)
        {
            PlayGasBlowOffSound();
        }

        if (m_gas_switch_toggle_press == 1)
        {
            PlayGasFlowSound();
        }

        if (m_compressor_toggle_press == 1)
        {
            PlayCompressorSound();
        }

        if (m_heat_increase_press == 1 || m_heat_decrease_press == 1)
        {
            PlayInFlightSound();
        }
    }

    void PlayWindSound()
    {
        m_WindSoundWave = BuildSoundWaveObject(m_WindSound, 0, 1.0, true);
    }

    void PlayBurnerFireSound()
    {
        m_BurnerFireSoundWave = BuildSoundWaveObject(m_BurnerFireSound, 0, 1.0, true);
    }

    void PlayIgniterSound()
    {
        m_IgniterSoundWave = BuildSoundWaveObject(m_IgniterSound, 0, 0.5, true);
    }

    void PlayGasBlowOffSound()
    {
        m_GasBlowOffSoundWave = BuildSoundWaveObject(m_GasBlowOffSound, 0, 1.0, false);
    }

    void PlayGasFlowSound()
    {
        m_GasFlowSoundWave = BuildSoundWaveObject(m_GasFlowSound, 0, 0.5, true);
    }

    void PlayCompressorSound()
    {
        m_CompressorSoundWave = BuildSoundWaveObject(m_CompressorSound, 0, 0.5, true);
    }

    void PlayInFlightSound()
    {
        m_InFlightSoundWave = BuildSoundWaveObject(m_InFlightSound, 0, 0.5, true);
    }

    void UpdateSounds(float dt)
    {
        if (!GetGame().IsServer()) return;

        SoundTimer += dt;

        if (SoundTimer >= 0.5)
        {
            UpdateSoundEffects();
            SoundTimer = 0;
        }

        if (m_WindSoundWave)
        {
            m_WindSoundWave.SetPosition(GetPosition());
        }

        if (m_BurnerFireSoundWave)
        {
            m_BurnerFireSoundWave.SetPosition(GetPosition());
        }

        if (m_IgniterSoundWave)
        {
            m_IgniterSoundWave.SetPosition(GetPosition());
        }

        if (m_GasBlowOffSoundWave)
        {
            m_GasBlowOffSoundWave.SetPosition(GetPosition());
        }

        if (m_GasFlowSoundWave)
        {
            m_GasFlowSoundWave.SetPosition(GetPosition());
        }

        if (m_CompressorSoundWave)
        {
            m_CompressorSoundWave.SetPosition(GetPosition());
        }

        if (m_InFlightSoundWave)
        {
            m_InFlightSoundWave.SetPosition(GetPosition());
        }
    }

    void updatePosition(string memoryPointName)
    {
        if (MemoryPointExists(memoryPointName))
        {
            vector point = GetMemoryPointPos(memoryPointName);
            vector world = ModelToWorld(point);
            vector ground = world;

            ground[1] = GetGame().SurfaceY(ground[0], ground[2]);

            float undermap = vector.Distance(world, ground);

            if (world[1] < (ground[1] - 0.20))
            {
                vector newPosition = GetPosition();
                newPosition[1] = newPosition[1] + undermap;
                SetPosition(newPosition);
            }
        }
    }

    void MapFallFix()
    {
        if (GetGame().IsServer())
        {
            updatePosition("map_break_front");
            updatePosition("map_break_left");
            updatePosition("map_break_right");
        }
    }

    override void OnEngineStart()
    {
        super.OnEngineStart();

        HandleBalloon();
    }

    override void OnEngineStop()
    {
        super.OnEngineStop();

        HandleBalloon();
    }

    void HandleBalloon()
    {
        if (m_IsBalloon)
        {
            if (m_IsEngineOn)
            {
                StartInflation();
            }
            else
            {
                StartDeflation();
            }
        }
    }

// ------------------------------

    void OnDestroy()
    {
        if (!GetGame().IsServer()) return;

        if (m_WindSoundWave)	m_WindSoundWave.Stop();
        if (m_BurnerFireSoundWave)	m_BurnerFireSoundWave.Stop();
        if (m_IgniterSoundWave)	m_IgniterSoundWave.Stop();
        if (m_GasBlowOffSoundWave)	m_GasBlowOffSoundWave.Stop();
        if (m_GasFlowSoundWave)	m_GasFlowSoundWave.Stop();
        if (m_CompressorSoundWave)	m_CompressorSoundWave.Stop();
        if (m_InFlightSoundWave)	m_InFlightSoundWave.Stop();
    }

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

// ------------------------------

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

// ------------------------------

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

// ------------------------------

    override void OnDebugSpawn()
    {
        EntityAI entity;

        if (Class.CastTo(entity, this))
        {
            entity.GetInventory().CreateAttachment("CarBattery");
            entity.GetInventory().CreateAttachment("CarRadiator");
            entity.GetInventory().CreateAttachment("SparkPlug");
            //entity.GetInventory().CreateAttachment("smalldummywheel");
            //entity.GetInventory().CreateAttachment("smalldummywheel");
            //entity.GetInventory().CreateAttachment("smalldummywheel");
            //entity.GetInventory().CreateAttachment("smalldummywheel");
        };

        Fill(CarFluid.FUEL, 130.0);
        Fill(CarFluid.OIL, 4.0);
    }
};

class HotAirBalloon: Balloon_Base {};
