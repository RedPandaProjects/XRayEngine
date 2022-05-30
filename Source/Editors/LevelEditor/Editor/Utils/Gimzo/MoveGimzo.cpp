#include "stdafx.h"
#include "MoveGimzo.h"

MoveGimzo::MoveGimzo()
{
    m_bVisible = false;
    m_CurrentStatus = EStatus::None;
}

MoveGimzo::~MoveGimzo()
{
}

void MoveGimzo::Render()
{
    if (!m_bVisible)return;
    
    RCache.set_Z(false);
    Fvector Start = m_Position;
    Fvector X; X.set(1, 0, 0); X.add(m_Position);
    Fvector Y; Y.set(0, 1, 0); Y.add(m_Position);
    Fvector Z; Z.set(0, 0, 1); Z.add(m_Position);

    DU_impl.DrawLine(Start, X, color_rgba(m_CurrentStatus == EStatus::SelectedX ?255:127, 0, 0, 255));
    DU_impl.DrawLine(Start, Y, color_rgba(0, m_CurrentStatus == EStatus::SelectedY ? 255 : 127, 0, 255));
    DU_impl.DrawLine(Start, Z, color_rgba(0, 0, m_CurrentStatus == EStatus::SelectedZ ? 255 : 127, 255));
     
    DU_impl.DrawCone(Fidentity, X, Fvector().set(-1, 0, 0), 0.1f, 0.03f, color_rgba(m_CurrentStatus == EStatus::SelectedX ? 255 : 127, 0, 0, 255), color_rgba(255, 0, 0, 255), TRUE, FALSE);
    DU_impl.DrawCone(Fidentity, Y, Fvector().set(0, -1, 0), 0.1f, 0.03f, color_rgba(0, m_CurrentStatus == EStatus::SelectedY ? 255 : 127, 0, 255), color_rgba(0, 255, 0, 255), TRUE, FALSE);
    DU_impl.DrawCone(Fidentity, Z, Fvector().set(0, 0, -1), 0.1f, 0.03f, color_rgba(0, 0, m_CurrentStatus == EStatus::SelectedZ ? 255 : 127, 255), color_rgba(0, 0, 255, 255), TRUE, FALSE);

    RCache.set_Z(true);
}

void MoveGimzo::OnFrame()
{

    Fbox Box; Box.invalidate();
    size_t SelectedCount = 0;

    if (OBJCLASS_DUMMY == LTools->CurrentClassID())
    {
        SceneToolsMapPairIt _I = Scene->FirstTool();
        SceneToolsMapPairIt _E = Scene->LastTool();
        for (; _I != _E; _I++)
        {
            ESceneCustomOTool* ObjectTool = _I->second->CastObjectTool();
            if ((_I->first != OBJCLASS_DUMMY) && ObjectTool)
            {
                for (auto& Object : ObjectTool->GetObjects())
                {
                    if (!Object->Selected())continue;
                    m_Position = Object->GetPosition();
                    Fbox ObjectBox;
                    if (Object->GetBox(ObjectBox))
                    {
                        Box.merge(ObjectBox);
                    }
                    SelectedCount++;
                }
            }
        }
    }
    else {
        ESceneToolBase* mt = Scene->GetTool(LTools->CurrentClassID());
        if (mt)
        {
            ESceneCustomOTool* ObjectTool = mt->CastObjectTool();
            if (ObjectTool)
            {
                for (auto& Object : ObjectTool->GetObjects())
                {
                    if (!Object->Selected())continue;
                    m_Position = Object->GetPosition();
                    Fbox ObjectBox;
                    if (Object->GetBox(ObjectBox))
                    {
                        Box.merge(ObjectBox);
                    }
                    SelectedCount++;
                }
            }
        }
    }
    if (SelectedCount > 1)
    {
        Box.getcenter(m_Position);
    }
    m_bVisible = SelectedCount > 0;
    m_CurrentStatus = EStatus::None;

    if (!m_bVisible)return;

    Fbox XBox;
    Fbox YBox;
    Fbox ZBox;

    XBox.set(0, -0.03, -0.03, 1, 0.03, 0.03);
    XBox.offset(m_Position);
    YBox.set(-0.03, 0, -0.03, 0.03, 1, 0.03);
    YBox.offset(m_Position);
    ZBox.set(-0.03, -0.03, 0, 0.03, 0.03, 1);
    ZBox.offset(m_Position);
    if (XBox.Pick(UI->m_CurrentRStart, UI->m_CurrentRDir))
        m_CurrentStatus = EStatus::SelectedX;
    else  if (YBox.Pick(UI->m_CurrentRStart, UI->m_CurrentRDir))
        m_CurrentStatus = EStatus::SelectedY;
    else  if (ZBox.Pick(UI->m_CurrentRStart, UI->m_CurrentRDir))
        m_CurrentStatus = EStatus::SelectedZ;
}
