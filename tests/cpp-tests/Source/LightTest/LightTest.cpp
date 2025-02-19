/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 https://axmol.dev/

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

#include "LightTest.h"

using namespace ax;

LightTests::LightTests()
{
    ADD_TEST_CASE(LightTest);
}

LightTest::LightTest() : _directionalLight(nullptr), _pointLight(nullptr), _spotLight(nullptr)
{
    addSprite();
    addLights();
    scheduleUpdate();

    auto s      = Director::getInstance()->getWinSize();
    auto camera = Camera::createPerspective(60, (float)s.width / s.height, 1.0f, 1000.0f);
    camera->setCameraFlag(CameraFlag::USER1);
    camera->setPosition3D(Vec3(0.0f, 100.0f, 100.0f));
    camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
    addChild(camera);

    TTFConfig ttfConfig("fonts/arial.ttf", 15);
    _ambientLightLabel = Label::createWithTTF(ttfConfig, "Ambient Light ON");
    _ambientLightLabel->retain();
    auto menuItem0 =
        MenuItemLabel::create(_ambientLightLabel, AX_CALLBACK_1(LightTest::SwitchLight, this, LightType::AMBIENT));
    _directionalLightLabel = Label::createWithTTF(ttfConfig, "Directional Light OFF");
    _directionalLightLabel->retain();
    auto menuItem1   = MenuItemLabel::create(_directionalLightLabel,
                                             AX_CALLBACK_1(LightTest::SwitchLight, this, LightType::DIRECTIONAL));
    _pointLightLabel = Label::createWithTTF(ttfConfig, "Point Light OFF");
    _pointLightLabel->retain();
    auto menuItem2 =
        MenuItemLabel::create(_pointLightLabel, AX_CALLBACK_1(LightTest::SwitchLight, this, LightType::POINT));
    _spotLightLabel = Label::createWithTTF(ttfConfig, "Spot Light OFF");
    _spotLightLabel->retain();
    auto menuItem3 =
        MenuItemLabel::create(_spotLightLabel, AX_CALLBACK_1(LightTest::SwitchLight, this, LightType::SPOT));
    auto menu = Menu::create(menuItem0, menuItem1, menuItem2, menuItem3, nullptr);
    menu->setPosition(Vec2::ZERO);
    menuItem0->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    menuItem0->setPosition(Vec2(VisibleRect::left().x, VisibleRect::top().y - 50));
    menuItem1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    menuItem1->setPosition(Vec2(VisibleRect::left().x, VisibleRect::top().y - 100));
    menuItem2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    menuItem2->setPosition(Vec2(VisibleRect::left().x, VisibleRect::top().y - 150));
    menuItem3->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    menuItem3->setPosition(Vec2(VisibleRect::left().x, VisibleRect::top().y - 200));
    addChild(menu);
}

LightTest::~LightTest()
{
    if (_spotLightLabel)
        _spotLightLabel->release();

    if (_pointLightLabel)
        _pointLightLabel->release();

    if (_directionalLightLabel)
        _directionalLightLabel->release();

    if (_spotLight)
        _spotLight->release();

    if (_pointLight)
        _pointLight->release();

    if (_directionalLight)
        _directionalLight->release();

    if (_ambientLight)
        _ambientLight->release();
}

std::string LightTest::title() const
{
    return "Light Test";
}

void LightTest::addSprite()
{
    auto s = Director::getInstance()->getWinSize();

    //{
    //    std::string fileName = "MeshRendererTest/plane.c3b";
    //    auto sprite = MeshRenderer::create(fileName);
    //    sprite->setRotation3D(Vec3(-90.0, 0.0, 0.0));
    //    sprite->setScale(5.0f);
    //    sprite->setPosition(Vec2(0.0, -50.0));
    //    addChild(sprite);
    //    sprite->setCameraMask(2);
    //}

    {
        std::string fileName = "MeshRendererTest/orc.c3b";
        auto mesh            = MeshRenderer::create(fileName);
        mesh->setRotation3D(Vec3(0.0f, 180.0f, 0.0f));
        mesh->setPosition(Vec2(0.0f, 0.0f));
        mesh->setScale(2.0);
        auto m = MeshRenderer::create("MeshRendererTest/axe.c3b");
        mesh->getAttachNode("Bip001 R Hand")->addChild(m);
        auto animation = Animation3D::create(fileName);
        if (animation)
        {
            auto animate = Animate3D::create(animation);
            mesh->runAction(RepeatForever::create(animate));
        }

        addChild(mesh);
        mesh->setCameraMask(2);
    }

    {
        std::string fileName = "MeshRendererTest/sphere.c3b";
        auto sprite          = MeshRenderer::create(fileName);
        sprite->setPosition(Vec2(30.0f, 0.0f));
        addChild(sprite);
        sprite->setCameraMask(2);
    }

    {
        std::string fileName = "MeshRendererTest/sphere.c3b";
        auto sprite          = MeshRenderer::create(fileName);
        sprite->setScale(0.5f);
        sprite->setPosition(Vec2(-50.0f, 0.0f));
        addChild(sprite);
        sprite->setCameraMask(2);
    }

    {
        std::string fileName = "MeshRendererTest/sphere.c3b";
        auto sprite          = MeshRenderer::create(fileName);
        sprite->setScale(0.5f);
        sprite->setPosition(Vec2(-30.0f, 10.0f));
        addChild(sprite);
        sprite->setCameraMask(2);
    }
}

void LightTest::addLights()
{
    auto s        = Director::getInstance()->getWinSize();
    _ambientLight = AmbientLight::create(Color3B(200, 200, 200));
    _ambientLight->retain();
    _ambientLight->setEnabled(true);
    addChild(_ambientLight);
    _ambientLight->setCameraMask(2);

    _directionalLight = DirectionLight::create(Vec3(-1.0f, -1.0f, 0.0f), Color3B(200, 200, 200));
    _directionalLight->retain();
    _directionalLight->setEnabled(false);
    addChild(_directionalLight);
    _directionalLight->setCameraMask(2);

    _pointLight = PointLight::create(Vec3(0.0f, 0.0f, 0.0f), Color3B(200, 200, 200), 10000.0f);
    _pointLight->retain();
    _pointLight->setEnabled(false);
    addChild(_pointLight);
    _pointLight->setCameraMask(2);

    _spotLight =
        SpotLight::create(Vec3(-1.0f, -1.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Color3B(200, 200, 200), 0.0, 0.5, 10000.0f);
    _spotLight->retain();
    _spotLight->setEnabled(false);
    addChild(_spotLight);
    _spotLight->setCameraMask(2);

    {
        auto tintto1 = TintTo::create(4, 0, 0, 255);
        auto tintto2 = TintTo::create(4, 0, 255, 0);
        auto tintto3 = TintTo::create(4, 255, 0, 0);
        auto tintto4 = TintTo::create(4, 255, 255, 255);
        auto seq     = Sequence::create(tintto1, tintto2, tintto3, tintto4, nullptr);
        _ambientLight->runAction(RepeatForever::create(seq));
    }

    {
        auto tintto1 = TintTo::create(4, 255, 0, 0);
        auto tintto2 = TintTo::create(4, 0, 255, 0);
        auto tintto3 = TintTo::create(4, 0, 0, 255);
        auto tintto4 = TintTo::create(4, 255, 255, 255);
        auto seq     = Sequence::create(tintto1, tintto2, tintto3, tintto4, nullptr);
        _directionalLight->runAction(RepeatForever::create(seq));
    }

    {
        auto tintto1 = TintTo::create(4, 255, 0, 0);
        auto tintto2 = TintTo::create(4, 0, 255, 0);
        auto tintto3 = TintTo::create(4, 0, 0, 255);
        auto tintto4 = TintTo::create(4, 255, 255, 255);
        auto seq     = Sequence::create(tintto2, tintto1, tintto3, tintto4, nullptr);
        _pointLight->runAction(RepeatForever::create(seq));
    }

    {
        auto tintto1 = TintTo::create(4, 255, 0, 0);
        auto tintto2 = TintTo::create(4, 0, 255, 0);
        auto tintto3 = TintTo::create(4, 0, 0, 255);
        auto tintto4 = TintTo::create(4, 255, 255, 255);
        auto seq     = Sequence::create(tintto3, tintto2, tintto1, tintto4, nullptr);
        _spotLight->runAction(RepeatForever::create(seq));
    }
}

void LightTest::update(float delta)
{
    static float angleDelta = 0.0;

    if (_directionalLight)
    {
        _directionalLight->setRotation3D(Vec3(-45.0, -AX_RADIANS_TO_DEGREES(angleDelta), 0.0f));
    }

    if (_pointLight)
    {
        _pointLight->setPositionX(100.0f * cosf(angleDelta + 2.0 * delta));
        _pointLight->setPositionY(100.0f);
        _pointLight->setPositionZ(100.0f * sinf(angleDelta + 2.0 * delta));
    }

    if (_spotLight)
    {
        _spotLight->setPositionX(100.0f * cosf(angleDelta + 4.0 * delta));
        _spotLight->setPositionY(100.0f);
        _spotLight->setPositionZ(100.0f * sinf(angleDelta + 4.0 * delta));
        _spotLight->setDirection(-Vec3(cosf(angleDelta + 4.0 * delta), 1.0, sinf(angleDelta + 4.0 * delta)));
    }

    angleDelta += delta;

    TestCase::update(delta);
}

void LightTest::SwitchLight(Object* sender, LightType lightType)
{
    switch (lightType)
    {
    case LightType::AMBIENT:
    {
        char str[32];
        bool isON = !_ambientLight->isEnabled();
        sprintf(str, "Ambient Light %s", isON == true ? "ON" : "OFF");
        _ambientLight->setEnabled(isON);
        _ambientLightLabel->setString(str);
    }
    break;

    case LightType::DIRECTIONAL:
    {
        char str[32];
        bool isON = !_directionalLight->isEnabled();
        sprintf(str, "Directional Light %s", isON == true ? "ON" : "OFF");
        _directionalLight->setEnabled(isON);
        _directionalLightLabel->setString(str);
    }
    break;

    case LightType::POINT:
    {
        char str[32];
        bool isON = !_pointLight->isEnabled();
        sprintf(str, "Point Light %s", isON == true ? "ON" : "OFF");
        _pointLight->setEnabled(isON);
        _pointLightLabel->setString(str);
    }
    break;

    case LightType::SPOT:
    {
        char str[32];
        bool isON = !_spotLight->isEnabled();
        sprintf(str, "Spot Light %s", isON == true ? "ON" : "OFF");
        _spotLight->setEnabled(isON);
        _spotLightLabel->setString(str);
    }
    break;

    default:
        break;
    }
}
