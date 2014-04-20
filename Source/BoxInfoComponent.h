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

#pragma once

//#include "Component.h"
#include "LogicComponent.h"

// All Urho3D classes reside in namespace Urho3D
using namespace Urho3D;

/// Custom component for rotating a scene node.
//class BoxInfoComponent : public Component
class BoxInfoComponent : public LogicComponent
{
    OBJECT(BoxInfoComponent);

public:
    /// Construct.
    BoxInfoComponent(Context* context);

    /// Cluster number
    int clusterId_;
    /// URL
    String label_;

protected:
    /// Handle node being assigned.
    virtual void OnNodeSet(Node* node);

private:
    /// Handle scene update event.
    void HandleSceneUpdate(StringHash eventType, VariantMap& eventData);
};
