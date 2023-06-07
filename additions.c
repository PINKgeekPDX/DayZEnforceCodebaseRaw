class Balloon_Base extends CarScript
{
    protected float m_BalloonVolume;
    protected float m_BalloonMaxVolume;

    protected float m_BalloonInflation;
    protected float m_BalloonMaxInflation;

    protected float m_BalloonDeflationRate;
    protected float m_BalloonInflationRate;

    protected float m_BalloonTemperature;
    protected float m_BalloonMaxTemperature;
    protected float m_BalloonTemperatureIncreaseRate;
    protected float m_BalloonTemperatureDecreaseRate;
    protected float m_BalloonTemperatureSafeZone;

    protected bool m_IsInflated;
    protected bool m_IsDeflated;

    protected bool m_IsTemperatureSafe;

    protected bool m_IsPhysicsOn;
    protected bool m_IsPhysicsInitialized;

    protected bool m_IsInitialized;
    protected bool m_IsDestroyed;

    void Balloon_Base()
    {
        m_BalloonVolume = 0;
        m_BalloonMaxVolume = 100;

        m_BalloonInflation = 0;
        m_BalloonMaxInflation = 100;

        m_BalloonDeflationRate = 5;
        m_BalloonInflationRate = 5;

        m_BalloonTemperature = 20;
        m_BalloonMaxTemperature = 80;
        m_BalloonTemperatureIncreaseRate = 1;
        m_BalloonTemperatureDecreaseRate = 2;
        m_BalloonTemperatureSafeZone = 30;

        m_IsInflated = false;
        m_IsDeflated = true;

        m_IsTemperatureSafe = true;

        m_IsPhysicsOn = false;
        m_IsPhysicsInitialized = false;

        m_IsInitialized = false;
        m_IsDestroyed = false;
    }

    void ~Balloon_Base()
    {
        DestroyBalloon();
    }

    void Init()
    {
        if (m_IsInitialized)
            return;

        super.Init();

        if (!GetGame().IsServer())
            return;
            
        if (!m_IsPhysicsInitialized)
        {
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(InitializePhysics, 1000, false);
            m_IsPhysicsInitialized = true;
        }
        m_IsInitialized = true;
    }

    void DestroyBalloon()
    {
        if (m_IsDestroyed)
            return;

        if (m_IsPhysicsInitialized)
            DestroyPhysics();

        m_IsDestroyed = true;
    }

    void InitializePhysics()
    {
        if (!GetGame().IsServer()) return;
        if (m_IsPhysicsOn) return;
        if (!m_IsInflated) return;
        if (m_BalloonVolume <= 0) return;

        vector position = GetPosition();
        vector force = "0 0 0";
        vector torque = "0 0 0";
        vector velocity = "0 0 0";
        vector angularVelocity = "0 0 0";
        vector impulse = "0 0 0";
        vector angularImpulse = "0 0 0";

        float mass = 100;
        m_BalloonVolume = m_BalloonInflation * m_BalloonMaxVolume / m_BalloonMaxInflation;

        if (m_BalloonVolume > 0) mass = m_BalloonVolume;

        dBodyCreateDynamic(this, 1.0);
        dBodySetPosition(this, position[0], position[1], position[2]);
        dBodySetMass(this, mass);
        dBodySetForce(this, force[0], force[1], force[2]);
        dBodySetTorque(this, torque[0], torque[1], torque[2]);
        dBodySetLinearVel(this, velocity[0], velocity[1], velocity[2]);
        dBodySetAngularVel(this, angularVelocity[0], angularVelocity[1], angularVelocity[2]);
        dBodyApplyImpulse(this, impulse[0], impulse[1], impulse[2]);
        dBodyApplyTorqueImpulse(this, angularImpulse[0], angularImpulse[1], angularImpulse[2]);

        m_IsPhysicsOn = true;
    }

    void DestroyPhysics()
    {
        if (!GetGame().IsServer()) return;
        if (!m_IsPhysicsOn) return;

        dBodyDestroy(this);

        m_IsPhysicsOn = false;
    }

    void OnUpdate(float deltaTime)
    {
        if (!GetGame().IsServer()) return;
        if (!m_IsInitialized) return;
        if (m_IsDestroyed) return;
        if (!m_IsInflated) return;

        if (m_IsPhysicsOn)
        {
            if (!m_IsTemperatureSafe) DestroyPhysics();
        }

 else
        {
            if (m_IsTemperatureSafe) InitializePhysics();
        }

        if (m_IsInflated)
        {
            if (m_BalloonTemperature < m_BalloonMaxTemperature)
                m_BalloonTemperature += m_BalloonTemperatureIncreaseRate * deltaTime;

            if (m_BalloonTemperature > m_BalloonMaxTemperature)
                m_BalloonTemperature = m_BalloonMaxTemperature;

            if (m_BalloonInflation < m_BalloonMaxInflation)
                m_BalloonInflation += m_BalloonInflationRate * deltaTime;

            if (m_BalloonInflation > m_BalloonMaxInflation)
                m_BalloonInflation = m_BalloonMaxInflation;

            if (m_BalloonInflation >= m_BalloonMaxInflation)
                m_IsDeflated = false;

            if (m_BalloonTemperature >= m_BalloonMaxTemperature)
                m_IsTemperatureSafe = false;
        }

        else
        {
            if (m_BalloonTemperature > 20)
                m_BalloonTemperature -= m_BalloonTemperatureDecreaseRate * deltaTime;

            if (m_BalloonTemperature < 20)
                m_BalloonTemperature = 20;

            if (m_BalloonInflation > 0)
                m_BalloonInflation -= m_BalloonDeflationRate * deltaTime;

            if (m_BalloonInflation < 0)
                m_BalloonInflation = 0;

            if (m_BalloonInflation <= 0)
                m_IsDeflated = true;

            if (m_BalloonTemperature <= m_BalloonTemperatureSafeZone)
                m_IsTemperatureSafe = true;
        }
    }

    void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);

        if (rpc_type == BALLOON_INFLATE)
        {
            if (!m_IsInflated && m_IsDeflated && m_IsTemperatureSafe)
            {
                m_IsInflated = true;
                m_IsDeflated = false;
            }
        }

        if (rpc_type == BALLOON_DEFLATE)
        {
            if (m_IsInflated && !m_IsDeflated)
            {
                m_IsInflated = false;
                m_IsDeflated = true;
            }
        }
    }

    bool IsInflated()
    {
        return m_IsInflated;
    }

    bool IsDeflated()
    {
        return m_IsDeflated;
    }

    bool IsTemperatureSafe()
    {
        return m_IsTemperatureSafe;
    }

    float GetBalloonTemperature()
    {
        return m_BalloonTemperature;
    }

    float GetBalloonInflation()
    {
        return m_BalloonInflation;
    }

    void Inflate()
    {
        if (!m_IsInflated && m_IsDeflated && m_IsTemperatureSafe)
        {
            m_IsInflated = true;
            m_IsDeflated = false;
            GetGame().RPCSingleParam(this, BALLOON_INFLATE, new Param1<bool>(m_IsInflated), true, GetGame().GetIdentity());
        }
    }

    void Deflate()
    {
        if (m_IsInflated && !m_IsDeflated)
        {
            m_IsInflated = false;
            m_IsDeflated = true;
            GetGame().RPCSingleParam(this, BALLOON_DEFLATE, new Param1<bool>(m_IsInflated), true, GetGame().GetIdentity());
        }
    }
}
