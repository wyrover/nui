#pragma once


namespace nui
{
    namespace Ui
    {
        class NCursor
        {
        public:
            virtual ~NCursor();

            enum CursorType
            {
                CursorArrow         = 0,
                CursorHand          = 1,
                CursorUpDown        = 2,
                CursorLeftRight     = 3,
                CursorCross         = 4,
                CursorSlash         = 5,
                CursorBackSlash     = 6,
            };
        };

        NUI_API NCursor* GetCursorByType(NCursor::CursorType type);
    }
}