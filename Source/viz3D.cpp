// Based on Urho3D sample 20_HugeObjectCount
// Author: Anthony Verez
// Thanks to Emmanuel Chaboud for his help

#include <stdlib.h>

#include "Button.h"
#include "Camera.h"
#include "CoreEvents.h"
#include "Engine.h"
#include "File.h"
#include "FileSystem.h"
#include "Font.h"
#include "Graphics.h"
#include "Input.h"
#include "Log.h"
#include "Material.h"
#include "Model.h"
#include "Octree.h"
#include "Profiler.h"
#include "Renderer.h"
#include "ResourceCache.h"
#include "Scene.h"
#include "StaticModelGroup.h"
#include "Text.h"
#include "UI.h"
#include "UIEvents.h"
#include "Vector.h"
#include "Window.h"
#include "Zone.h"

#include "viz3D.h"
#include "BoxInfoComponent.h"

#include "DebugNew.h"

DEFINE_APPLICATION_MAIN(Viz3D)

struct MATERIALS {
   static const String array[30];
};

Viz3D::Viz3D(Context* context) :
    Sample(context),
    animate_(false),
    uiRoot_(GetSubsystem<UI>()->GetRoot()),
    window_()
{
    // Register an object factory for our custom BoxInfoComponent component so that we can create them to scene nodes
    context->RegisterFactory<BoxInfoComponent>();
}

void Viz3D::Start()
{
    // Execute base class startup
    Sample::Start();

    // Enable OS cursor
    GetSubsystem<Input>()->SetMouseVisible(true);

    // Load XML file containing default UI style sheet
    SharedPtr<ResourceCache> cache(GetSubsystem<ResourceCache>());
    SharedPtr<XMLFile> style(cache->GetResource<XMLFile>("UI/DefaultStyle.xml"));

    // Set the loaded style as default style
    uiRoot_->SetDefaultStyle(style);

    // Initialize Window
    InitWindow();

    // Create and add some controls to the Window
    InitControls();

    // Hook up to the frame update events
    SubscribeToEvents();
}

void Viz3D::StartVisu()
{
    // Create the scene content
    CreateScene();

    // Create the UI content
    CreateInstructions();

    // Create clusterId and label info text placeholders
    CreatePointInfo();

    // Setup the viewport for displaying the scene
    SetupViewport();

    // Hook up to the frame update events
    SubscribeToEventsVisu();
}

void Viz3D::InitControls()
{
    Vector<String> strvector;
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    FileSystem* filesystem;
    filesystem->ScanDir(strvector, String("Data/Datasets/"), "", 1, true);

    // Create a Button
    for(int i=0; i < strvector.Size(); i++) {
        LOGINFO(strvector[i]);
        Button* button = new Button(context_);
        button->SetName(strvector[i]);
        button->SetMinHeight(24);
        window_->AddChild(button);
        button->SetStyleAuto();
        Text* buttonText = button->CreateChild<Text>();
        buttonText->SetAlignment(HA_CENTER, VA_CENTER);
        buttonText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 12);
        buttonText->SetText(strvector[i]);
    }
}

void Viz3D::InitWindow()
{
    // Create the Window and add it to the UI's root node
    window_ = SharedPtr<Window>(new Window(context_));
    uiRoot_->AddChild(window_);

    // Set Window size and layout settings
    window_->SetMinSize(384, 192);
    window_->SetLayout(LM_VERTICAL, 6, IntRect(6, 6, 6, 6));
    window_->SetAlignment(HA_CENTER, VA_CENTER);
    window_->SetName("Window");

    // Apply styles
    window_->SetStyleAuto();
}

void Viz3D::CreateScene()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();

    if (!scene_)
        scene_ = new Scene(context_);
    else
    {
        scene_->Clear();
        boxNodes_.Clear();
    }

    // Create the Octree component to the scene so that drawable objects can be rendered. Use default volume
    // (-1000, -1000, -1000) to (1000, 1000, 1000)
    scene_->CreateComponent<Octree>();

    GetSubsystem<Renderer>()->GetDefaultZone()->SetFogColor(Color(1.0f, 1.0f, 1.0f));
    GetSubsystem<Renderer>()->GetDefaultZone()->SetAmbientColor(Color(1.0f, 1.0f, 1.0f));
    // Create a Zone for ambient light & fog control
    /*Node* zoneNode = scene_->CreateChild("Zone");
    Zone* zone = zoneNode->CreateComponent<Zone>();
    zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
    zone->SetFogColor(Color(0.5f, 0.5f, 0.5f));
    zone->SetFogStart(200.0f);
    zone->SetFogEnd(300.0f);*/

    // Create a directional light
    // Node* lightNode = scene_->CreateChild("DirectionalLight");
    // lightNode->SetDirection(Vector3(-0.6f, -1.0f, -0.8f)); // The direction vector does not need to be normalized
    // Light* light = lightNode->CreateComponent<Light>();
    // light->SetLightType(LIGHT_DIRECTIONAL);

    // light->SetColor(Color(1.0f, 0.898f, 0.702f));

    // Load data
    File myfile(context_, String("Data/Datasets/")+map_);
    double x, y, z;
    int clusterId;
    String label;
    String line = myfile.ReadLine();
    int fieldsNumberTsv = line.Split('\t').Size();

    clusterEnabled_ = false;
    labelEnabled_ = false;

    for(int i=0; i < fieldsNumberTsv; i++) {
        if (line.Split('\t')[i] == String("cluster")) {
            clusterEnabled_ = true;
        }
        if (line.Split('\t')[i] == String("label")) {
            labelEnabled_ = true;
        }
    }
    LOGINFO(String("cluster field: ")+String(clusterEnabled_));
    LOGINFO(String("label field: ")+String(labelEnabled_));

    line = myfile.ReadLine();
    Vector<String> lineargs;
    StaticModelGroup* currentModelGroup;
    Vector<StaticModelGroup*> staticModelGroups;

    LOGINFO(String("Loading ")+map_+String("..."));

    // Initializing static model groups with colors
    for(int i=0; i<30; i++)
    {
        Node* boxGroupNode = scene_->CreateChild("BoxGroup");
        currentModelGroup = boxGroupNode->CreateComponent<StaticModelGroup>();
        currentModelGroup->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
        switch (i) {
            case 0:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Azure.xml"));
                break;
            case 1:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Cyan.xml"));
                break;
            case 2:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Aquamarine.xml"));
                break;
            case 3:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Teal.xml"));
                break;
            case 4:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/ForestGreen.xml"));
                break;
                case 5:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Silver.xml"));
                break;
            case 6:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/NavyBlue.xml"));
                break;
            case 7:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/RoyalBlue.xml"));
                break;
            case 8:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/MediumBlue.xml"));
                break;
            case 9:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Ivory.xml"));
                break;
            case 10:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Beige.xml"));
                break;
            case 11:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Wheat.xml"));
                break;
            case 12:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Tan.xml"));
                break;
            case 13:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Khaki.xml"));
                break;
            case 14:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Olive.xml"));
                break;
            case 15:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Chartreuse.xml"));
                break;
            case 16:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Lime.xml"));
                break;
            case 17:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Golden.xml"));
                break;
            case 18:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Goldenrod.xml"));
                break;
            case 19:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Coral.xml"));
                break;
            case 20:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Salmon.xml"));
                break;
            case 21:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/HotPink.xml"));
                break;
            case 22:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Fuchsia.xml"));
                break;
            case 23:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Puce.xml"));
                break;
            case 24:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Mauve.xml"));
                break;
            case 25:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Lavender.xml"));
                break;
            case 26:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Plum.xml"));
                break;
            case 27:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Indigo.xml"));
                break;
            case 28:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Maroon.xml"));
                break;
            case 29:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Crimson.xml"));
                break;
            default:
                currentModelGroup->SetMaterial(cache->GetResource<Material>("Materials/Blue.xml"));
        }
        staticModelGroups.Push(currentModelGroup);
    }

    while(!line.Empty())
    {
        lineargs = line.Split('\t');
        x = atof(lineargs[0].CString());
        y = atof(lineargs[1].CString());
        z = atof(lineargs[2].CString());
        if (clusterEnabled_ && !labelEnabled_)
            clusterId = atoi(lineargs[3].CString());
        if (labelEnabled_ && !clusterEnabled_)
            label = lineargs[3];
        if (clusterEnabled_ && labelEnabled_) {
            clusterId = atoi(lineargs[3].CString());
            label = lineargs[4];
        }

        Node* boxNode = scene_->CreateChild("Box");
        boxNode->SetPosition(Vector3(x * 400.0f, z * 400.0f, y * 400.0f));
        boxNode->SetScale(0.25f);
        BoxInfoComponent* myboxinfo = boxNode->CreateComponent<BoxInfoComponent>();
        if (clusterEnabled_)
            myboxinfo->clusterId_ = clusterId;
        if (labelEnabled_)
            myboxinfo->label_ = label;
        boxNodes_.Push(SharedPtr<Node>(boxNode));
        if (clusterEnabled_)
            staticModelGroups[clusterId]->AddInstanceNode(boxNode);
        else
            staticModelGroups[0]->AddInstanceNode(boxNode);

        line = myfile.ReadLine();
    }

    // Create the camera. Create it outside the scene so that we can clear the whole scene without affecting it
    if (!cameraNode_)
    {
        cameraNode_ = new Node(context_);
        cameraNode_->SetPosition(Vector3(0.0f, 10.0f, -100.0f));
        Camera* camera = cameraNode_->CreateComponent<Camera>();
        camera->SetFarClip(300.0f);
    }
}

void Viz3D::CreateInstructions()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    UI* ui = GetSubsystem<UI>();

    // Construct new Text object, set string to display and font to use
    Text* instructionText = ui->GetRoot()->CreateChild<Text>();
    instructionText->SetText(
        "Use WASD keys and mouse/touch to move\n"
        "Use Shift to accelerate\n"
        "Space to toggle animation"
    );
    instructionText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 15);
    // The text has multiple rows. Center them in relation to each other
    instructionText->SetTextAlignment(HA_CENTER);
    instructionText->SetColor(Color(0.0f, 0.0f, 0.0f));

    // Position the text relative to the screen center
    instructionText->SetHorizontalAlignment(HA_CENTER);
    instructionText->SetVerticalAlignment(VA_CENTER);
    instructionText->SetPosition(0, ui->GetRoot()->GetHeight() / 4);

    // Target image
    BorderImage* targetImage = ui->GetRoot()->CreateChild<BorderImage>();
    targetImage->SetTexture(cache->GetResource<Texture2D>("Textures/Sight.png"));
    targetImage->SetFullImageRect();
    targetImage->SetFixedSize(20, 20);
    targetImage->SetHorizontalAlignment(HA_CENTER);
    targetImage->SetVerticalAlignment(VA_CENTER);
}

void Viz3D::CreatePointInfo()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    UI* ui = GetSubsystem<UI>();

    // Construct new Text object, set string to display and font to use
    clusterIdText_ = ui->GetRoot()->CreateChild<Text>();
    clusterIdText_->SetText("");
    clusterIdText_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 15);
    clusterIdText_->SetColor(Color(0.0f, 0.0f, 0.0f));

    // Position the text on top left
    clusterIdText_->SetHorizontalAlignment(HA_LEFT);
    clusterIdText_->SetVerticalAlignment(VA_TOP);
    clusterIdText_->SetPosition(100, ui->GetRoot()->GetHeight() / 8);

    // Construct new Text object, set string to display and font to use
    labelText_ = ui->GetRoot()->CreateChild<Text>();
    labelText_->SetText("");
    labelText_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 15);
    labelText_->SetColor(Color(0.0f, 0.0f, 0.0f));

    // Position the text on top left
    labelText_->SetHorizontalAlignment(HA_LEFT);
    labelText_->SetVerticalAlignment(VA_TOP);
    labelText_->SetPosition(100, ui->GetRoot()->GetHeight() / 8 + 30);

}

void Viz3D::SetupViewport()
{
    Renderer* renderer = GetSubsystem<Renderer>();

    // Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
    SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
    renderer->SetViewport(0, viewport);
}

void Viz3D::SubscribeToEvents()
{
    // Subscribe handler; invoked whenever a mouse click event is dispatched
    SubscribeToEvent(E_UIMOUSECLICK, HANDLER(Viz3D, HandleControlClicked));
}

void Viz3D::SubscribeToEventsVisu()
{
    // Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, HANDLER(Viz3D, HandleUpdate));

    // Subscribe HandlePostRenderUpdate() function for processing the post-render update event, during which we request
    // debug geometry
    SubscribeToEvent(E_POSTRENDERUPDATE, HANDLER(Viz3D, HandlePostRenderUpdate));
}

void Viz3D::MoveCamera(float timeStep)
{
    // Do not move if the UI has a focused element (the console)
    // if (GetSubsystem<UI>()->GetFocusElement()) {
    //     LOGINFO(String("Err, focused element"));
    //     return;
    // }

    Input* input = GetSubsystem<Input>();

    // Movement speed as world units per second
    float move_speed = 20.0f;
    // Mouse sensitivity as degrees per pixel
    const float MOUSE_SENSITIVITY = 0.1f;

    // Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees
    IntVector2 mouseMove = input->GetMouseMove();
    yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
    pitch_ += MOUSE_SENSITIVITY * mouseMove.y_;
    pitch_ = Clamp(pitch_, -90.0f, 90.0f);

    // Construct new orientation for the camera scene node from yaw and pitch. Roll is fixed to zero
    cameraNode_->SetRotation(Quaternion(pitch_, yaw_, 0.0f));

    // Read WASD keys and move the camera scene node to the corresponding direction if they are pressed
    // Acceleration
    if (input->GetKeyDown(KEY_LSHIFT))
        move_speed = 60.0f;
    if (input->GetKeyDown('W'))
        cameraNode_->Translate(Vector3::FORWARD * move_speed * timeStep);
    if (input->GetKeyDown('S'))
        cameraNode_->Translate(Vector3::BACK * move_speed * timeStep);
    if (input->GetKeyDown('A'))
        cameraNode_->Translate(Vector3::LEFT * move_speed * timeStep);
    if (input->GetKeyDown('D'))
        cameraNode_->Translate(Vector3::RIGHT * move_speed * timeStep);

    // Set route start/endpoint with left mouse button, recalculate route if applicable
    if (input->GetMouseButtonPress(MOUSEB_LEFT))
        SetPathPoint();
}

void Viz3D::SetPathPoint()
{
    Vector3 hitPos;
    Node* hitNode;

    if (Raycast(250.0f, hitPos, hitNode))
    {
        targetedPos_ = hitPos;
        targetedNode_ = hitNode;
        targetedDefined_ = true;
    }
    else
        targetedDefined_ = false;
}

bool Viz3D::Raycast(float maxDistance, Vector3& hitPos, Node*& hitNode)
{
    hitNode = 0;

    UI* ui = GetSubsystem<UI>();
    IntVector2 pos = ui->GetCursorPosition();
    // Check the cursor is visible and there is no UI element in front of the cursor
    if (ui->GetElementAt(pos, true))
        return false;

    Graphics* graphics = GetSubsystem<Graphics>();
    Camera* camera = cameraNode_->GetComponent<Camera>();
    Ray cameraRay = camera->GetScreenRay((float)pos.x_ / graphics->GetWidth(), (float)pos.y_ / graphics->GetHeight());
    // Pick only geometry objects, not eg. zones or lights, only get the first (closest) hit
    PODVector<RayQueryResult> results;
    //RayOctreeQuery query(results, cameraRay, RAY_TRIANGLE, maxDistance, DRAWABLE_GEOMETRY);
    RayOctreeQuery query(results, cameraRay, RAY_AABB, maxDistance, DRAWABLE_GEOMETRY);
    scene_->GetComponent<Octree>()->RaycastSingle(query);
    if (results.Size())
    {
        RayQueryResult& result = results[0];

        // Calculate hit position in world space
        hitPos = cameraRay.origin_ + cameraRay.direction_ * result.distance_;
        Node* drawableNode = result.drawable_->GetNode();

        if (drawableNode->HasComponent<StaticModelGroup>())
        {
            hitNode = drawableNode->GetComponent<StaticModelGroup>()->GetInstanceNode(result.subObject_);
            return true;
        }
    }

    return false;
}

void Viz3D::AnimateObjects(float timeStep)
{
    PROFILE(AnimateObjects);

    const float ROTATE_SPEED = 30.0f;
    // Rotate about the Z axis (roll)
    Quaternion rotateQuat(ROTATE_SPEED * timeStep, Vector3::FORWARD);

    for (unsigned i = 0; i < boxNodes_.Size(); ++i)
        boxNodes_[i]->Rotate(rotateQuat);
}

void Viz3D::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;

    // Take the frame time step, which is stored as a float
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    // Toggle animation with space
    Input* input = GetSubsystem<Input>();
    if (input->GetKeyPress(KEY_SPACE))
        animate_ = !animate_;

    // Move the camera, scale movement with time step
    MoveCamera(timeStep);

    // Animate scene if enabled
    if (animate_)
        AnimateObjects(timeStep);
}

void Viz3D::HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData)
{
    SetPathPoint();
    // Visualize the targeted point
    if (targetedDefined_)
    {
        BoxInfoComponent* boxInfoComponent = targetedNode_->GetComponent<BoxInfoComponent>();
        if (clusterEnabled_)
            clusterIdText_->SetText(String("Cluster ")+String(boxInfoComponent->clusterId_));
        if (labelEnabled_)
            labelText_->SetText(boxInfoComponent->label_);
    }
    else
    {
        clusterIdText_->SetText("");
        labelText_->SetText("");
    }
}

void Viz3D::HandleControlClicked(StringHash eventType, VariantMap& eventData)
{
    String name;
    // Get control that was clicked
    UIElement* clicked = static_cast<UIElement*>(eventData[UIMouseClick::P_ELEMENT].GetPtr());

    if (clicked)
    {
        // Get the name of the control that was clicked
        name = clicked->GetName();

        if (strcmp(name.CString(), "...?") && strcmp(name.CString(), "Window")) {
            map_ = name;
            Viz3D::StartVisu();
            UnsubscribeFromEvent(E_UIMOUSECLICK);
            GetSubsystem<Input>()->SetMouseVisible(false);
            uiRoot_->RemoveChild(window_);
        }
    }
}
