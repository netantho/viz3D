//
// Copyright (c) 2008-2013 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include <iostream>
#include <string>

#include "BoxInfoComponent.h"
#include "Scene.h"
#include "SceneEvents.h"

#include "DebugNew.h"

BoxInfoComponent::BoxInfoComponent(Context* context) :
    LogicComponent(context)
{
}

// void BoxInfoComponent::SetClusterId(std::string clusterId)
// {
//     clusterId_ = clusterId;
// }

// const std::string BoxInfoComponent::GetClusterId()
// {
//     return(clusterId_);
// }

// void BoxInfoComponent::SetUrl(std::string url)
// {
//     url_ = url;
// }

// const std::string BoxInfoComponent::GetUrl()
// {
//     return(url_);
// }

void BoxInfoComponent::OnNodeSet(Node* node)
{
    // If the node pointer is nonzero, this component has been created into a scene node. Subscribe to the variable timestep
    // scene update event now. If the node pointer is zero, the component is being removed from a scene node at destruction
    // time. In that case we do nothing
    if (node)
    {
        Scene* scene = node->GetScene();
        // The scene pointer will be nonzero if the scene node belongs to a scene (it is also legal to create free-standing
        // scene nodes)
        if (scene)
            SubscribeToEvent(scene, E_SCENEUPDATE, HANDLER(BoxInfoComponent, HandleSceneUpdate));
    }
}

void BoxInfoComponent::HandleSceneUpdate(StringHash eventType, VariantMap& eventData)
{

}
