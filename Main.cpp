#include <plugin.h> // Plugin-SDK version 1000 from 2025-10-28 18:33:25

#include "Main.h"

#include <CLines.h>
#include <CTheScripts.h>
#include <CWorld.h>

using namespace plugin;
using namespace injector;

static void(__fastcall* CRunningScript__UpdateCompareFlag)(CRunningScript*, int, uint8_t);
static void(__fastcall* CRunningScript__LocateCharCommand)(CRunningScript* self, int, int32_t CurrCommand);
static void(__fastcall* CRunningScript__LocateCharCharCommand)(CRunningScript* self, int, int32_t CurrCommand);
static void(__fastcall* CRunningScript__LocateCharCarCommand)(CRunningScript* self, int, int32_t CurrCommand);
static void(__fastcall* CRunningScript__LocateCharObjectCommand)(CRunningScript* self, int, int32_t CurrCommand);
static void(__fastcall* CRunningScript__CharInAreaCheckCommand)(CRunningScript* self, int, int32_t CurrCommand);
static void(__fastcall* CRunningScript__CarInAreaCheckCommand)(CRunningScript* self, int, int32_t CurrCommand);
static void(__fastcall* CRunningScript__ObjectInAreaCheckCommand)(CRunningScript* self, int, int32_t CurrCommand);

//#define RENDER_OVERLAY

struct Main
{
    Main()
    {
        // COMMAND_IS_CHAR_IN_AREA_2D
        CRunningScript__UpdateCompareFlag = MakeCALL(0x467951, raw_ptr(CRunningScript__UpdateCompareFlag_DrawSquare)).get();
        // COMMAND_IS_CAR_IN_AREA_2D
        CRunningScript__UpdateCompareFlag = MakeCALL(0x467F68, raw_ptr(CRunningScript__UpdateCompareFlag_DrawSquare)).get();
        // COMMAND_IS_CHAR_SHOOTING_IN_AREA
        CRunningScript__UpdateCompareFlag = MakeCALL(0x4801A3, raw_ptr(CRunningScript__UpdateCompareFlag_DrawSquare)).get();

        // COMMAND_IS_CHAR_IN_AREA_3D
        CRunningScript__UpdateCompareFlag = MakeCALL(0x467A4D, raw_ptr(CRunningScript__UpdateCompareFlag_DrawCube)).get();
        // COMMAND_IS_CAR_IN_AREA_3D
        CRunningScript__UpdateCompareFlag = MakeCALL(0x468019, raw_ptr(CRunningScript__UpdateCompareFlag_DrawCube)).get();

        // LocateCharCommand (0x00EC - 0x00F1 ; 0x00FE-0x0103 ranges)
        CRunningScript__LocateCharCommand = MakeCALL(0x469754, raw_ptr(CRunningScript__LocateCharCommand_Hook)).get();
        // LocateCharCharCommand (0x00F2 - 0x00F4 ; 0x0104 - 0x0106 ranges)
        CRunningScript__LocateCharCharCommand = MakeCALL(0x469763, raw_ptr(CRunningScript__LocateCharCharCommand_Hook)).get();
        // LocateCharCarCommand (0x0202 - 0x207 range)
        CRunningScript__LocateCharCarCommand = MakeCALL(0x47E36A, raw_ptr(CRunningScript__LocateCharCarCommand_Hook)).get();
        // LocateCharObjectCommand (0x0471 - 0x0476 range)
        CRunningScript__LocateCharObjectCommand = MakeCALL(0x48AB1C, raw_ptr(CRunningScript__LocateCharObjectCommand_Hook)).get();

        // CharInAreaCheckCommand (0x01A1 - 0x01AA range)
        CRunningScript__CharInAreaCheckCommand = MakeCALL(0x47D2B0, raw_ptr(CRunningScript__CharInAreaCheckCommand_Hook)).get();
        // CharInAreaCheckCommand (0x01AB, 0x01AC commands)
        CRunningScript__CarInAreaCheckCommand = MakeCALL(0x47D2CE, raw_ptr(CRunningScript__CarInAreaCheckCommand_Hook)).get();
        // CharInAreaCheckCommand (0x04E9, 0x04EA commands)
        CRunningScript__ObjectInAreaCheckCommand = MakeCALL(0x48C2EC, raw_ptr(CRunningScript__ObjectInAreaCheckCommand_Hook)).get();

        // 0x53E222 - RenderFirstPersonCar
        plugin::Events::renderEffectsEvent += CTheScripts__RenderTheScriptDebugLines;
    }

    static void CTheScripts__RenderTheScriptDebugLines() {

#ifdef RENDER_OVERLAY
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
#else
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)true);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)true);
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
#endif // RENDER_AFTER_2D

        for (int i = 0; i < NumScriptDebugLines; i++)
        {
            auto line = aStoredLines[i];
            CLines::RenderLineWithClipping(line.vecInf.x, line.vecInf.y, line.vecInf.z, line.vecSup.x, line.vecSup.y, line.vecSup.z, line.color1, line.color2);
        }

        NumScriptDebugLines = 0;

#ifdef RENDER_OVERLAY
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)FALSE);
#else
        RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)5);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)6);
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)false);
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)true);
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)true);
#endif // RENDER_OVERLAY
    }

    static void CTheScripts__DrawDebugSquare(float x1, float y1, float x2, float y2)
    {
        CColPoint colPoint{};
        CEntity* colEntity;

        CVector p1{ x1, y1, -1000.0f };
        CWorld::ProcessVerticalLine(p1, 1000.0f, colPoint, colEntity, true, false, false, false, true, false, nullptr);
        p1.z = colPoint.m_vecPoint.z + 2.0f;

        CVector p2{ x2, y1, -1000.0f };
        CWorld::ProcessVerticalLine(p2, 1000.0f, colPoint, colEntity, true, false, false, false, true, false, nullptr);
        p2.z = colPoint.m_vecPoint.z + 2.0f;

        CVector p3{ x2, y2, -1000.0f };
        CWorld::ProcessVerticalLine(p3, 1000.0f, colPoint, colEntity, true, false, false, false, true, false, nullptr);
        p3.z = colPoint.m_vecPoint.z + 2.0f;

        CVector p4{ x1, y2, -1000.0f };
        CWorld::ProcessVerticalLine(p4, 1000.0f, colPoint, colEntity, true, false, false, false, true, false, nullptr);
        p4.z = colPoint.m_vecPoint.z + 2.0f; // FIX_BUGS: missing in original code 🤷

        ScriptDebugLine3D(p1, p2, DbgLineColour, DbgLineColour);
        ScriptDebugLine3D(p2, p3, DbgLineColour, DbgLineColour);
        ScriptDebugLine3D(p3, p4, DbgLineColour, DbgLineColour);
        ScriptDebugLine3D(p4, p1, DbgLineColour, DbgLineColour);
    }

    static void CTheScripts__DrawDebugCube(const CVector& inf, const CVector& sup) {
        ScriptDebugLine3D({ inf.x, inf.y, inf.z }, { sup.x, inf.y, inf.z }, DbgLineColour, DbgLineColour);
        ScriptDebugLine3D({ sup.x, inf.y, inf.z }, { sup.x, sup.y, inf.z }, DbgLineColour, DbgLineColour);
        ScriptDebugLine3D({ sup.x, sup.y, inf.z }, { inf.x, sup.y, inf.z }, DbgLineColour, DbgLineColour);
        ScriptDebugLine3D({ inf.x, sup.y, inf.z }, { inf.x, inf.y, inf.z }, DbgLineColour, DbgLineColour);
        ScriptDebugLine3D({ inf.x, inf.y, sup.z }, { sup.x, inf.y, sup.z }, DbgLineColour, DbgLineColour);
        ScriptDebugLine3D({ sup.x, inf.y, sup.z }, { sup.x, sup.y, sup.z }, DbgLineColour, DbgLineColour);
        ScriptDebugLine3D({ sup.x, sup.y, sup.z }, { inf.x, sup.y, sup.z }, DbgLineColour, DbgLineColour);
        ScriptDebugLine3D({ inf.x, sup.y, sup.z }, { inf.x, inf.y, sup.z }, DbgLineColour, DbgLineColour);
        ScriptDebugLine3D({ inf.x, inf.y, sup.z }, { inf.x, inf.y, inf.z }, DbgLineColour, DbgLineColour);
        ScriptDebugLine3D({ sup.x, inf.y, sup.z }, { sup.x, inf.y, inf.z }, DbgLineColour, DbgLineColour);
        ScriptDebugLine3D({ sup.x, sup.y, sup.z }, { sup.x, sup.y, inf.z }, DbgLineColour, DbgLineColour);
        ScriptDebugLine3D({ inf.x, sup.y, sup.z }, { inf.x, sup.y, inf.z }, DbgLineColour, DbgLineColour);
    }

    static void ScriptDebugLine3D(const CVector& start, const CVector& end, uint32_t color1, uint32_t color2)
    {
        if (NumScriptDebugLines >= MAX_NUM_STORED_LINES)
            return;

        auto& line = aStoredLines[NumScriptDebugLines];
        line.vecInf = start;
        line.vecSup = end;
        line.color1 = color1;
        line.color2 = color2;
        NumScriptDebugLines++;
    }

    static void __fastcall CRunningScript__UpdateCompareFlag_DrawSquare(CRunningScript* self, int, uint8_t LatestResult)
    {
        //if(CTheScripts::DbgFlag)
        {
            CTheScripts__DrawDebugSquare(*((float*)&ScriptParams[1]), *((float*)&ScriptParams[2]), *((float*)&ScriptParams[3]), *((float*)&ScriptParams[4]));
        }

        return CRunningScript__UpdateCompareFlag(self, (int)nullptr, LatestResult);
    }

    static void __fastcall CRunningScript__UpdateCompareFlag_DrawCube(CRunningScript* self, int, uint8_t LatestResult)
    {
        //if(CTheScripts::DbgFlag)
        {
            CTheScripts__DrawDebugCube({ *((float*)&ScriptParams[1]), *((float*)&ScriptParams[2]), *((float*)&ScriptParams[3]) },
                { *((float*)&ScriptParams[4]), *((float*)&ScriptParams[5]), *((float*)&ScriptParams[6]) });
        }

        return CRunningScript__UpdateCompareFlag(self, (int)nullptr, LatestResult);
    }

    static void __fastcall CRunningScript__LocateCharCommand_Hook(CRunningScript* self, int, int32_t CurrCommand)
    {
        CRunningScript__LocateCharCommand(self, (int)nullptr, CurrCommand);

        bool b3D = false;
        float X, Y, Z, dX, dY, dZ;
        
        switch (CurrCommand)
        {
        case COMMAND_LOCATE_CHAR_ANY_MEANS_3D:
        case COMMAND_LOCATE_CHAR_ON_FOOT_3D:
        case COMMAND_LOCATE_CHAR_IN_CAR_3D:
        case COMMAND_LOCATE_STOPPED_CHAR_ANY_MEANS_3D:
        case COMMAND_LOCATE_STOPPED_CHAR_ON_FOOT_3D:
        case COMMAND_LOCATE_STOPPED_CHAR_IN_CAR_3D:
            b3D = true;
            break;
        default:
            b3D = false;
            break;
        }
        
        X = *(float*)&ScriptParams[1];
        Y = *(float*)&ScriptParams[2];

        if (b3D) {
            Z = *(float*)&ScriptParams[3];
            dX = *(float*)&ScriptParams[4];
            dY = *(float*)&ScriptParams[5];
            dZ = *(float*)&ScriptParams[6];
        }
        else {
            dX = *(float*)&ScriptParams[3];
            dY = *(float*)&ScriptParams[4];
        }

        //if (CTheScripts::DbgFlag)
        {
            if (b3D)
                CTheScripts__DrawDebugCube({ X - dX, Y - dY, Z - dZ }, { X + dX, Y + dY, Z + dZ });
            else
                CTheScripts__DrawDebugSquare(X - dX, Y - dY, X + dX, Y + dY);
        }
    }

    static void __fastcall CRunningScript__LocateCharCharCommand_Hook(CRunningScript* self, int, int32_t CurrCommand)
    {
        CRunningScript__LocateCharCharCommand(self, (int)nullptr, CurrCommand);

        bool b3D = false;
        float X, Y, Z, dX, dY, dZ;

        switch (CurrCommand)
        {
        case COMMAND_LOCATE_CHAR_ANY_MEANS_CHAR_3D:
        case COMMAND_LOCATE_CHAR_ON_FOOT_CHAR_3D:
        case COMMAND_LOCATE_CHAR_IN_CAR_CHAR_3D:
            b3D = true;
            break;
        default:
            b3D = false;
            break;
        }

        CPed* pTarget = CPools::ms_pPedPool->GetAt(ScriptParams[1] >> 8);

        if (pTarget->bInVehicle) {
            X = pTarget->m_pVehicle->GetPosition().x;
            Y = pTarget->m_pVehicle->GetPosition().y;
            Z = pTarget->m_pVehicle->GetPosition().z;
        }
        else {
            X = pTarget->GetPosition().x;
            Y = pTarget->GetPosition().y;
            Z = pTarget->GetPosition().z;
        }

        dX = *(float*)&ScriptParams[2];
        dY = *(float*)&ScriptParams[3];

        if (b3D)
            dZ = *(float*)&ScriptParams[4];

        //if (CTheScripts::DbgFlag) 
        {
            if (b3D)
                CTheScripts__DrawDebugCube({ X - dX, Y - dY, Z - dZ }, { X + dX, Y + dY, Z + dZ });
            else
                CTheScripts__DrawDebugSquare(X - dX, Y - dY, X + dX, Y + dY);
        }
    }

    static void __fastcall CRunningScript__LocateCharCarCommand_Hook(CRunningScript* self, int, int32_t CurrCommand)
    {
        CRunningScript__LocateCharCarCommand(self, (int)nullptr, CurrCommand);

        bool b3D = false;
        float X, Y, Z, dX, dY, dZ;

        switch (CurrCommand)
        {
        case COMMAND_LOCATE_CHAR_ANY_MEANS_CAR_3D:
        case COMMAND_LOCATE_CHAR_ON_FOOT_CAR_3D:
        case COMMAND_LOCATE_CHAR_IN_CAR_CAR_3D:
            b3D = true;
            break;
        default:
            b3D = false;
            break;
        }

        CVehicle* pTarget = CPools::ms_pVehiclePool->GetAt(ScriptParams[1] >> 8);

        X = pTarget->GetPosition().x;
        Y = pTarget->GetPosition().y;
        Z = pTarget->GetPosition().z;

        dX = *(float*)&ScriptParams[2];
        dY = *(float*)&ScriptParams[3];

        if (b3D)
            dZ = *(float*)&ScriptParams[4];

        //if (CTheScripts::DbgFlag) 
        {
            if (b3D)
                CTheScripts__DrawDebugCube({ X - dX, Y - dY, Z - dZ }, { X + dX, Y + dY, Z + dZ });
            else
                CTheScripts__DrawDebugSquare(X - dX, Y - dY, X + dX, Y + dY);
        }
    }

    static void __fastcall CRunningScript__LocateCharObjectCommand_Hook(CRunningScript* self, int, int32_t CurrCommand)
    {
        CRunningScript__LocateCharObjectCommand(self, (int)nullptr, CurrCommand);

        bool b3D = false;
        float X, Y, Z, dX, dY, dZ;

        switch (CurrCommand)
        {
        case COMMAND_LOCATE_CHAR_ANY_MEANS_OBJECT_3D:
        case COMMAND_LOCATE_CHAR_ON_FOOT_OBJECT_3D:
        case COMMAND_LOCATE_CHAR_IN_CAR_OBJECT_3D:
            b3D = true;
            break;
        default:
            b3D = false;
            break;
        }

        CObject* pTarget = CPools::ms_pObjectPool->GetAt(ScriptParams[1] >> 8);

        X = pTarget->GetPosition().x;
        Y = pTarget->GetPosition().y;
        Z = pTarget->GetPosition().z;

        dX = *(float*)&ScriptParams[2];
        dY = *(float*)&ScriptParams[3];

        if (b3D)
            dZ = *(float*)&ScriptParams[4];

        //if (CTheScripts::DbgFlag) 
        {
            if (b3D)
                CTheScripts__DrawDebugCube({ X - dX, Y - dY, Z - dZ }, { X + dX, Y + dY, Z + dZ });
            else
                CTheScripts__DrawDebugSquare(X - dX, Y - dY, X + dX, Y + dY);
        }
    }

    static void __fastcall CRunningScript__CharInAreaCheckCommand_Hook(CRunningScript* self, int, int32_t CurrCommand)
    {
        CRunningScript__CharInAreaCheckCommand(self, (int)nullptr, CurrCommand);

        bool b3D = false;
        float infX, infY, infZ, supX, supY, supZ;

        switch (CurrCommand)
        {
        case COMMAND_IS_CHAR_IN_AREA_3D: // Not related!!!
        case COMMAND_IS_CHAR_IN_AREA_ON_FOOT_3D:
        case COMMAND_IS_CHAR_IN_AREA_IN_CAR_3D:
        case COMMAND_IS_CHAR_STOPPED_IN_AREA_3D:
        case COMMAND_IS_CHAR_STOPPED_IN_AREA_ON_FOOT_3D:
        case COMMAND_IS_CHAR_STOPPED_IN_AREA_IN_CAR_3D:
            b3D = true;
            break;
        default:
            b3D = false;
            break;
        }

        infX = *(float*)&ScriptParams[1];
        infY = *(float*)&ScriptParams[2];
        if (b3D) {
            infZ = *(float*)&ScriptParams[3];
            supX = *(float*)&ScriptParams[4];
            supY = *(float*)&ScriptParams[5];
            supZ = *(float*)&ScriptParams[6];
            if (infZ > supZ) {
                infZ = *(float*)&ScriptParams[6];
                supZ = *(float*)&ScriptParams[3];
            }
        }
        else {
            supX = *(float*)&ScriptParams[3];
            supY = *(float*)&ScriptParams[4];
        }
        if (infX > supX) {
            float tmp = infX;
            infX = supX;
            supX = tmp;
        }
        if (infY > supY) {
            float tmp = infY;
            infY = supY;
            supY = tmp;
        }

        //if (CTheScripts::DbgFlag) 
        {
            if (b3D)
                CTheScripts__DrawDebugCube({ infX, infY, infZ }, { supX, supY, supZ });
            else
                CTheScripts__DrawDebugSquare(infX, infY, supX, supY);
        }
    }

    static void __fastcall CRunningScript__CarInAreaCheckCommand_Hook(CRunningScript* self, int, int32_t CurrCommand)
    {
        CRunningScript__CarInAreaCheckCommand(self, (int)nullptr, CurrCommand);

        bool b3D = false;
        float infX, infY, infZ, supX, supY, supZ;

        if (CurrCommand == COMMAND_IS_CAR_IN_AREA_3D || CurrCommand == COMMAND_IS_CAR_STOPPED_IN_AREA_3D) // This is stupid cuz COMMAND_IS_CAR_IN_AREA_3D is not related 
            b3D = true;
        
        infX = *(float*)&ScriptParams[1];
        infY = *(float*)&ScriptParams[2];

        if (b3D) {
            infZ = *(float*)&ScriptParams[3];
            supX = *(float*)&ScriptParams[4];
            supY = *(float*)&ScriptParams[5];
            supZ = *(float*)&ScriptParams[6];
            if (infZ > supZ) {
                infZ = *(float*)&ScriptParams[6];
                supZ = *(float*)&ScriptParams[3];
            }
        }
        else {
            supX = *(float*)&ScriptParams[3];
            supY = *(float*)&ScriptParams[4];
        }

        //if (CTheScripts::DbgFlag) 
        {
            if (b3D)
                CTheScripts__DrawDebugCube({ infX, infY, infZ }, { supX, supY, supZ });
            else
                CTheScripts__DrawDebugSquare(infX, infY, supX, supY);
        }
    }

    static void __fastcall CRunningScript__ObjectInAreaCheckCommand_Hook(CRunningScript* self, int, int32_t CurrCommand)
    {
        CRunningScript__ObjectInAreaCheckCommand(self, (int)nullptr, CurrCommand);

        bool b3D = false;
        float infX, infY, infZ, supX, supY, supZ;

        if (CurrCommand == COMMAND_IS_OBJECT_IN_AREA_3D)
            b3D = true;

        infX = *(float*)&ScriptParams[1];
        infY = *(float*)&ScriptParams[2];
        if (b3D) {
            infZ = *(float*)&ScriptParams[3];
            supX = *(float*)&ScriptParams[4];
            supY = *(float*)&ScriptParams[5];
            supZ = *(float*)&ScriptParams[6];
            if (infZ > supZ) {
                infZ = *(float*)&ScriptParams[6];
                supZ = *(float*)&ScriptParams[3];
            }
        }
        else {
            supX = *(float*)&ScriptParams[3];
            supY = *(float*)&ScriptParams[4];
        }
        if (infX > supX) {
            float tmp = infX;
            infX = supX;
            supX = tmp;
        }
        if (infY > supY) {
            float tmp = infY;
            infY = supY;
            supY = tmp;
        }

        //if (CTheScripts::DbgFlag) 
        {
            if (b3D)
                CTheScripts__DrawDebugCube({ infX, infY, infZ }, { supX, supY, supZ });
            else
                CTheScripts__DrawDebugSquare(infX, infY, supX, supY);
        }
    }
} gInstance;
