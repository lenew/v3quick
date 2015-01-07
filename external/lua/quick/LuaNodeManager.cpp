/****************************************************************************
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "LuaNodeManager.h"
#include "base/CCDirector.h"

#include "base/ccUtils.h"
#include "base/CCEventDispatcher.h"

NS_CC_BEGIN

static LuaNodeManager* s_sharedLuaNodeManager = nullptr;

LuaNodeManager* LuaNodeManager::getInstance()
{
    if (s_sharedLuaNodeManager == nullptr)
    {
        s_sharedLuaNodeManager = new (std::nothrow) LuaNodeManager();
        if (!s_sharedLuaNodeManager->init())
        {
            delete s_sharedLuaNodeManager;
            s_sharedLuaNodeManager = nullptr;
            CCLOG("ERROR: Could not init LuaNodeManager");
        }
    }
    return s_sharedLuaNodeManager;
}

void LuaNodeManager::destroyInstance()
{
    //    log("---> LuaNodeManager::destroyInstance");
    CC_SAFE_DELETE(s_sharedLuaNodeManager);
}

LuaNodeManager::LuaNodeManager()
{
    _luaNodes.reserve(100);
}

LuaNodeManager::~LuaNodeManager()
{
    //    log("---> Release LuaNodeManager");
    if (_luaNodes.size() > 0)
    {
        _luaNodes.clear();
    }
}

bool LuaNodeManager::init()
{
    return true;
}

LuaEventNode* LuaNodeManager::getLuaNodeByNode(Node *node, bool toCreate)
{
    LuaEventNode *lnode = nullptr;
	if (node->getComponent(LuaEventNodeComponent::LUA_NODE_MANAGER_COMPONENT_NAME) != NULL) {
		lnode = ((LuaEventNodeComponent*)node->getComponent(LuaEventNodeComponent::LUA_NODE_MANAGER_COMPONENT_NAME))->getLuaEventNode();
		if (lnode && lnode->getActiveNode() == node) {
			return lnode;
		}
		else {
			node->removeComponent(LuaEventNodeComponent::LUA_NODE_MANAGER_COMPONENT_NAME);
        }
    }
    if (!lnode && toCreate)
    {
        lnode = LuaEventNode::create(node);
		Component* c = (Component*)new LuaEventNodeComponent(lnode);
		c->init();
		node->addComponent(c);
		c->autorelease();
    }
    return lnode;
}

void LuaNodeManager::removeLuaNode(LuaEventNode *lnode)
{
	//removeComponent may deconstruct lnode, so call it before removeComponent
	Node *detach = lnode->getDetachedNode();
	Node *active = lnode->getActiveNode();
	if (detach) detach->removeComponent(LuaEventNodeComponent::LUA_NODE_MANAGER_COMPONENT_NAME);
	if (active) active->removeComponent(LuaEventNodeComponent::LUA_NODE_MANAGER_COMPONENT_NAME);
}

NS_CC_END
