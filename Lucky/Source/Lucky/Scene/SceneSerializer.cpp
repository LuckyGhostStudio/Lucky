#include "lcpch.h"
#include "SceneSerializer.h"

#include "Object.h"

#include "Lucky/Scene/Components/SelfComponent.h"
#include "Lucky/Scene/Components/TransformComponent.h"
#include "Lucky/Scene/Components/SpriteRendererComponent.h"
#include "Lucky/Scene/Components/CameraComponent.h"
#include "Lucky/Scene/Components/Rigidbody2DComponent.h"
#include "Lucky/Scene/Components/BoxCollider2DComponent.h"
#include "Lucky/Scene/Components/CircleCollider2DComponent.h"
#include "Lucky/Scene/Components/ScriptComponent.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace YAML
{
    /// <summary>
    /// vec2 转换
    /// </summary>
    template<>
    struct convert<glm::vec2>
    {
        /// <summary>
        /// 将 vec2 转换为 YAML 的节点
        /// </summary>
        /// <param name="rhs">vec2 类型</param>
        /// <returns>结点</returns>
        static Node encode(const glm::vec2& rhs)
        {
            Node node;

            node.push_back(rhs.x);
            node.push_back(rhs.y);

            node.SetStyle(EmitterStyle::Flow);

            return node;
        }

        /// <summary>
        /// 将 YAML 结点类型转换为 vec2
        /// </summary>
        /// <param name="node">结点</param>
        /// <param name="rhs">vec2</param>
        /// <returns>是否转换成功</returns>
        static bool decode(const Node& node, glm::vec2& rhs)
        {
            if (!node.IsSequence() || node.size() != 2)
            {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();

            return true;
        }
    };

    /// <summary>
    /// vec3 转换
    /// </summary>
    template<>
    struct convert<glm::vec3>
    {
        /// <summary>
        /// 将 vec3 转换为 YAML 的节点
        /// </summary>
        /// <param name="rhs">vec3 类型</param>
        /// <returns>结点</returns>
        static Node encode(const glm::vec3& rhs)
        {
            Node node;

            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);

            node.SetStyle(EmitterStyle::Flow);

            return node;
        }

        /// <summary>
        /// 将 YAML 结点类型转换为 vec3
        /// </summary>
        /// <param name="node">结点</param>
        /// <param name="rhs">vec3</param>
        /// <returns>是否转换成功</returns>
        static bool decode(const Node& node, glm::vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
            {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();

            return true;
        }
    };

    /// <summary>
    /// vec4 转换
    /// </summary>
    template<>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& rhs)
        {
            Node node;

            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);

            node.SetStyle(EmitterStyle::Flow);

            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
            {
                return false;
            }

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();

            return true;
        }
    };
}

namespace Lucky
{
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
    {
        out << YAML::Flow;    // 流 [x,y]
        out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;

        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;

        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;

        return out;
    }

    SceneSerializer::SceneSerializer(const Ref<Scene> scene)
        : m_Scene(scene)
    {

    }

    /// <summary>
    /// 序列化物体
    /// </summary>
    /// <param name="out">发射器</param>
    /// <param name="object">物体</param>
    static void SerializeObject(YAML::Emitter& out, Object object)
    {
        out << YAML::BeginMap;  // 开始物体 Map
        out << YAML::Key << "Object" << YAML::Value << object.GetUUID();

        // Self 组件
        if (object.HasComponent<SelfComponent>())
        {
            out << YAML::Key << "SelfComponent";
            out << YAML::BeginMap;  // 开始 Self 组件 Map

            SelfComponent& selfComponent = object.GetComponent<SelfComponent>(); // Self 组件

            out << YAML::Key << "Name" << YAML::Value << selfComponent.Name;

            out << YAML::EndMap;    // 结束 Self 组件 Map
        }

        // Transform 组件
        if (object.HasComponent<TransformComponent>())
        {
            out << YAML::Key << "TransformComponent";
            out << YAML::BeginMap;  // 开始 Transform 组件 Map

            // Transform 组件数据
            TransformComponent& transformComponent = object.GetComponent<TransformComponent>();
            Transform& transform = transformComponent.Transform;

            out << YAML::Key << "Position" << YAML::Value << transform.GetPosition();
            out << YAML::Key << "Rotation" << YAML::Value << transform.GetRotation();
            out << YAML::Key << "Scale" << YAML::Value << transform.GetScale();

            out << YAML::EndMap;    // 结束 Transform 组件 Map
        }

        // Camera 组件
        if (object.HasComponent<CameraComponent>())
        {
            out << YAML::Key << "CameraComponent";
            out << YAML::BeginMap;  // 开始 Camera 组件 Map

            CameraComponent& cameraComponent = object.GetComponent<CameraComponent>();
            SceneCamera& camera = cameraComponent.Camera;

            out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;

            out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
            out << YAML::Key << "Size" << YAML::Value << camera.GetSize();
            out << YAML::Key << "FOV" << YAML::Value << camera.GetFOV();
            out << YAML::Key << "Near" << YAML::Value << camera.GetNearClip();
            out << YAML::Key << "Far" << YAML::Value << camera.GetFarClip();

            out << YAML::EndMap;    // 结束 Camera 组件 Map
        }

        // SpriteRenderer 组件
        if (object.HasComponent<SpriteRendererComponent>())
        {
            out << YAML::Key << "SpriteRendererComponent";
            out << YAML::BeginMap;  // 开始 SpriteRenderer 组件 Map

            SpriteRendererComponent& spriteRendererComponent = object.GetComponent<SpriteRendererComponent>();

            out << YAML::Key << "Sprite" << YAML::Value << spriteRendererComponent.Sprite->GetPath();
            out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;

            out << YAML::EndMap;    // 结束 SpriteRenderer 组件 Map
        }

        // Rigidbody2D 组件
        if (object.HasComponent<Rigidbody2DComponent>())
        {
            out << YAML::Key << "Rigidbody2DComponent";
            out << YAML::BeginMap;  // Rigidbody2DComponent

            Rigidbody2DComponent& rigidbody2DComponent = object.GetComponent<Rigidbody2DComponent>();
            Rigidbody2D& rigidbody2D = rigidbody2DComponent.Rigidbody2d;

            out << YAML::Key << "BodyType" << YAML::Value << (int)rigidbody2D.GetBodyType();

            out << YAML::Key << "Mass" << YAML::Value << rigidbody2D.GetMass();
            out << YAML::Key << "LinearDrag" << YAML::Value << rigidbody2D.GetLinearDrag();
            out << YAML::Key << "AngularDrag" << YAML::Value << rigidbody2D.GetAngularDrag();
            out << YAML::Key << "GravityScale" << YAML::Value << rigidbody2D.GetGravityScale();

            out << YAML::Key << "CollisionDetectionMode" << YAML::Value << (int)rigidbody2D.GetCollisionDetectionMode();

            out << YAML::Key << "FreezeRotation" << YAML::Value << rigidbody2D.IsFreezeRotation();

            out << YAML::EndMap;    // Rigidbody2DComponent
        }

        // BoxCollider2D 组件
        if (object.HasComponent<BoxCollider2DComponent>())
        {
            out << YAML::Key << "BoxCollider2DComponent";
            out << YAML::BeginMap;  // BoxCollider2DComponent

            BoxCollider2DComponent& boxCollider2DComponent = object.GetComponent<BoxCollider2DComponent>();
            BoxCollider2D& boxCollider2D = boxCollider2DComponent.BoxCollider2d;

            out << YAML::Key << "Offset" << YAML::Value << boxCollider2D.GetOffset();
            out << YAML::Key << "Size" << YAML::Value << boxCollider2D.GetSize();

            out << YAML::Key << "Density" << YAML::Value << boxCollider2D.GetDensity();
            out << YAML::Key << "Friction" << YAML::Value << boxCollider2D.GetFriction();
            out << YAML::Key << "Restitution" << YAML::Value << boxCollider2D.GetRestitution();
            out << YAML::Key << "RestitutionThreshold" << YAML::Value << boxCollider2D.GetRestitutionThreshold();

            out << YAML::EndMap;    // BoxCollider2DComponent
        }

        // CircleCollider2D 组件
        if (object.HasComponent<CircleCollider2DComponent>())
        {
            out << YAML::Key << "CircleCollider2DComponent";
            out << YAML::BeginMap;  // CircleCollider2DComponent

            CircleCollider2DComponent& circleCollider2DComponent = object.GetComponent<CircleCollider2DComponent>();
            CircleCollider2D& circleCollider2D = circleCollider2DComponent.CircleCollider2d;

            out << YAML::Key << "Offset" << YAML::Value << circleCollider2D.GetOffset();
            out << YAML::Key << "Radius" << YAML::Value << circleCollider2D.GetRadius();

            out << YAML::Key << "Density" << YAML::Value << circleCollider2D.GetDensity();
            out << YAML::Key << "Friction" << YAML::Value << circleCollider2D.GetFriction();
            out << YAML::Key << "Restitution" << YAML::Value << circleCollider2D.GetRestitution();
            out << YAML::Key << "RestitutionThreshold" << YAML::Value << circleCollider2D.GetRestitutionThreshold();

            out << YAML::EndMap;    // CircleCollider2DComponent
        }

        // Script 组件
        if (object.HasComponent<ScriptComponent>())
        {
            out << YAML::Key << "ScriptComponent";
            out << YAML::BeginMap;  // ScriptComponent

            ScriptComponent& scriptComponent = object.GetComponent<ScriptComponent>();
            
            out << YAML::Key << "ClassNamespace" << YAML::Value << scriptComponent.ClassNamespace;
            out << YAML::Key << "ClassName" << YAML::Value << scriptComponent.ClassName;
            // TODO public 成员变量

            out << YAML::EndMap;    // ScriptComponent
        }

        out << YAML::EndMap;    // 结束物体 Map
    }

    void SceneSerializer::Serialize(const std::string& filepath)
    {
        YAML::Emitter out;      // 发射器

        out << YAML::BeginMap;  // 开始场景 Map
        out << YAML::Key << "Scene" << YAML::Value << "Untitled";       // 场景：场景名
        out << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq; // 物体序列：开始物体序列

        // 遍历场景注册表所有物体
        m_Scene->m_Registry.each([&](auto objectID)
        {
            Object object = { objectID, m_Scene.get() };
            if (!object)
            {
                return;
            }

            SerializeObject(out, object);   // 序列化物体
        });

        out << YAML::EndSeq;    // 结束物体序列
        out << YAML::EndMap;    // 结束场景 Map

        std::ofstream fout(filepath);   // 输出流
        fout << out.c_str();            // 输出序列化结果到输出流文件
    }

    void SceneSerializer::SerializeRuntime(const std::string& filepath)
    {
        LC_CORE_ASSERT(false, "Not implemented!");  // 未实现
    }

    bool SceneSerializer::Deserialize(const std::string& filepath)
    {
        YAML::Node data = YAML::LoadFile(filepath); // 加载到 YMAL 结点

        // Scene 节点不存在
        if (!data["Scene"])
        {
            return false;
        }

        std::string sceneName = data["Scene"].as<std::string>();    // 场景名

        LC_CORE_TRACE("Deserializing scene '{0}'", sceneName);

        YAML::Node objects = data["Objects"];   // 物体序列结点

        // 物体序列结点存在
        if (objects)
        {
            // 遍历结点下所有物体
            for (auto object : objects)
            {
                uint64_t uuid = object["Object"].as<uint64_t>();    // UUID
                
                std::string objectName;

                // Self 组件结点
                YAML::Node selfComponentNode = object["SelfComponent"];
                if (selfComponentNode)
                {
                    objectName = selfComponentNode["Name"].as<std::string>(); // 物体名
                    // TODO ObjectEnable
                }

                LC_CORE_TRACE("Deserialized Object: [UUID = {0}, Name = {1}]", uuid, objectName);

                Object deserializedObject = m_Scene->CreateObject(uuid, objectName);  // 创建物体

                // Transform 组件结点
                YAML::Node transformComponentNode = object["TransformComponent"];
                if (transformComponentNode)
                {
                    TransformComponent& transformComponent = deserializedObject.GetComponent<TransformComponent>();  // 获取 Transform 组件

                    Transform& transform = transformComponent.Transform;

                    transform.SetPosition(transformComponentNode["Position"].as<glm::vec3>());
                    transform.SetRotation(transformComponentNode["Rotation"].as<glm::vec3>());
                    transform.SetScale(transformComponentNode["Scale"].as<glm::vec3>());
                }

                // Camera 组件结点
                YAML::Node cameraComponentNode = object["CameraComponent"];
                if (cameraComponentNode)
                {
                    CameraComponent& cameraComponent = deserializedObject.AddComponent<CameraComponent>();  // 添加 Camera 组件

                    SceneCamera& camera = cameraComponent.Camera;

                    cameraComponent.Primary = cameraComponentNode["Primary"].as<bool>();

                    camera.SetProjectionType((SceneCamera::ProjectionType)cameraComponentNode["ProjectionType"].as<int>());

                    camera.SetSize(cameraComponentNode["Size"].as<float>());
                    camera.SetFOV(cameraComponentNode["FOV"].as<float>());
                    camera.SetNearClip(cameraComponentNode["Near"].as<float>());
                    camera.SetFarClip(cameraComponentNode["Far"].as<float>());
                }

                // SpriteRenderer 组件结点
                YAML::Node spriteRendererComponentNode = object["SpriteRendererComponent"];
                if (spriteRendererComponentNode)
                {
                    SpriteRendererComponent& spriteRendererComponent = deserializedObject.AddComponent<SpriteRendererComponent>();  // 添加 SpriteRenderer 组件

                    const std::string spritePath = spriteRendererComponentNode["Sprite"].as<std::string>();
                    if (spritePath != "")
                    {
                        spriteRendererComponent.Sprite = Texture2D::Create(spritePath);
                    }
                    spriteRendererComponent.Color = spriteRendererComponentNode["Color"].as<glm::vec4>();
                }

                // Rigidbody2D 组件结点
                YAML::Node rigidbody2DNode = object["Rigidbody2DComponent"];
                if (rigidbody2DNode)
                {
                    Rigidbody2DComponent& rigidbody2DComponent = deserializedObject.AddComponent<Rigidbody2DComponent>();   // 添加 Rigidbody2D 组件
                    Rigidbody2D& rigidbody2D = rigidbody2DComponent.Rigidbody2d;

                    rigidbody2D.SetBodyType((Rigidbody2D::BodyType)rigidbody2DNode["BodyType"].as<int>());

                    rigidbody2D.SetMass(rigidbody2DNode["Mass"].as<float>());
                    rigidbody2D.SetLinearDrag(rigidbody2DNode["LinearDrag"].as<float>());
                    rigidbody2D.SetAngularDrag(rigidbody2DNode["AngularDrag"].as<float>());
                    rigidbody2D.SetGravityScale(rigidbody2DNode["GravityScale"].as<float>());

                    rigidbody2D.SetCollisionDetectionMode((Rigidbody2D::CollisionDetectionMode)rigidbody2DNode["CollisionDetectionMode"].as<int>());

                    rigidbody2D.FreezeRotation(rigidbody2DNode["FreezeRotation"].as<bool>());
                }

                // BoxCollider2D 组件结点
                YAML::Node boxCollider2DNode = object["BoxCollider2DComponent"];
                if (boxCollider2DNode)
                {
                    BoxCollider2DComponent& boxCollider2DComponent = deserializedObject.AddComponent<BoxCollider2DComponent>(); // 添加 BoxCollider2D 组件
                    BoxCollider2D& boxCollider2D = boxCollider2DComponent.BoxCollider2d;

                    boxCollider2D.SetOffset(boxCollider2DNode["Offset"].as<glm::vec2>());
                    boxCollider2D.SetSize(boxCollider2DNode["Size"].as<glm::vec2>());

                    boxCollider2D.SetDensity(boxCollider2DNode["Density"].as<float>());
                    boxCollider2D.SetFriction(boxCollider2DNode["Friction"].as<float>());
                    boxCollider2D.SetRestitution(boxCollider2DNode["Restitution"].as<float>());
                    boxCollider2D.SetRestitutionThreshold(boxCollider2DNode["RestitutionThreshold"].as<float>());
                }

                // CircleCollider2D 组件结点
                YAML::Node circleCollider2DNode = object["CircleCollider2DComponent"];
                if (circleCollider2DNode)
                {
                    CircleCollider2DComponent& circleCollider2DComponent = deserializedObject.AddComponent<CircleCollider2DComponent>(); // 添加 BoxCollider2D 组件
                    CircleCollider2D& circleCollider2D = circleCollider2DComponent.CircleCollider2d;

                    circleCollider2D.SetOffset(circleCollider2DNode["Offset"].as<glm::vec2>());
                    circleCollider2D.SetRadius(circleCollider2DNode["Radius"].as<float>());

                    circleCollider2D.SetDensity(circleCollider2DNode["Density"].as<float>());
                    circleCollider2D.SetFriction(circleCollider2DNode["Friction"].as<float>());
                    circleCollider2D.SetRestitution(circleCollider2DNode["Restitution"].as<float>());
                    circleCollider2D.SetRestitutionThreshold(circleCollider2DNode["RestitutionThreshold"].as<float>());
                }

                // ScriptComponent 组件结点
                YAML::Node scriptComponentNode = object["ScriptComponent"];
                if (scriptComponentNode)
                {
                    ScriptComponent& scriptComponent = deserializedObject.AddComponent<ScriptComponent>();  // 添加 Script 组件

                    scriptComponent.ClassNamespace = scriptComponentNode["ClassNamespace"].as<std::string>();
                    scriptComponent.ClassName = scriptComponentNode["ClassName"].as<std::string>();
                    // TODO public 成员变量
                }
            }
        }

        return true;
    }

    bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
    {
        LC_CORE_ASSERT(false, "Not implemented!");  // 未实现

        return false;
    }
}