#pragma once


#include "./NRender.h"
#include "../base/BaseObj.h"
#include "../base/NString.h"
#include "../base/DataTypes.h"
#include "../base/NAutoPtr.h"
#include "../data/NArrayT.h"
#include "NWindow.h"

namespace nui
{
    namespace Ui
    {
        /**
        *note: child don't increase refCount of parent
        * consider such situation:
        *  NInstPtr<NFrameBase> parent;
        *  NInstPtr<NFrameBase> child;
        *  parent->AddChild(child);
        *  parent = NULL;
        *  Expect: these two NFrameBases are released.
        *  In fact, they are not, because refCount of them are 1 at last.
        */

        BEGIN_USE_UNEXPORT_TEMPLATE()
        class NUI_CLASS NWindow;
        class NUI_CLASS NFrameBase;
        class NUI_CLASS NFrame;
        typedef FastDelegate2<NFrameBase*, LPARAM, bool> UiContainerEnumCallback;

        class NUI_CLASS NFrameBase : public nui::Base::NBaseObj
        {
            friend class NWindow;

        public:
            typedef std::list<NFrameBase*> FrameList;

        protected:
            enum Flag
            {
                FlagNone        = 0x0000,
                FlagVisible     = 0x0001,
                FlagValid       = 0x0002,
                FlagAutoSize    = 0x0004,
                FlagCanHover    = 0x0010,
                FlagLayoutable  = 0x0020,
            };

        public:
            enum Status
            {
                StatusNormal  = 0x0000,
                StatusHover   = 0x0001,
                StatusPressed = 0x0002,
                StatusDisabled= 0x0004,
                StatusChecked = 0x0008,
            };

            enum Layout
            {
                LayoutNone      = 0x0000,
                LayoutLeft      = 0x0001,
                LayoutTop       = 0x0002,
                LayoutRight     = 0x0004,
                LayoutBottom    = 0x0008,

                LayoutHCenter   = 0x0010,
                LayoutVCenter   = 0x0020,
                LayoutPosEnd    = 0x0020,

                LayoutSizeStart = 0x0040,
                LayoutHFill     = 0x0040,
                LayoutVFill     = 0x0080,
            };

            static bool IsVertLayout(UINT layout);
            static bool IsHorzLayout(UINT layout);

            NFrameBase();

        public:
            ~NFrameBase();

        public:

            // childs manipulations
            virtual bool AddChild(NFrameBase* child);
            virtual bool RemoveChild(NFrameBase* child);
            virtual void RemoveAllChilds();
            virtual size_t SetChildZOrder(NFrameBase* child, size_t zorder);
            virtual size_t GetChildZOrder(NFrameBase* child) const;
            virtual void SetChildTopmost(NFrameBase* child);
            virtual void SetChildBottommost(NFrameBase* child);
            virtual bool EnumChilds(UiContainerEnumCallback callback, LPARAM lParam) const;
            virtual NFrameBase* GetChildById(const Base::NString& id, bool recursive);
            virtual NFrameBase* GetChildByPointAndFlag(const Base::NPoint& point, DWORD flags, bool pointOffsetToParent);
            virtual size_t GetChildCount() const;

            virtual NFrameBase* GetParent() const;
            virtual void Invalidate() const;
            virtual void ForceInvalidate() const;
            virtual void Draw(NRender* render, Base::NPoint& ptOffset, HRGN clipRgn);

            // flags
            virtual bool SetVisible(bool visible);
            virtual bool IsVisible() const;
            virtual bool SetEnabled(bool enabled);
            virtual bool IsEnabled() const;
            virtual bool SetCheck(bool checked);
            virtual bool IsChecked() const;
            virtual bool IsInStatus(Status status) const;

            // data
            virtual void SetId(const Base::NString& id);
            virtual const Base::NString& GetId() const;
            virtual NFrame* SetData(DWORD data);
            virtual DWORD GetData() const;
            virtual NWindow* GetWindow();

            // pos / size
            virtual const Base::NRect& GetRect() const;
            virtual Base::NRect GetRootRect() const;
            virtual Base::NRect GetScreenRect() const;
            virtual bool SetPos(int left, int top);
            virtual bool SetSize(int width, int height);
            virtual bool SetMinSize(int minWidth, int minHeight);
            virtual bool SetMaxSize(int maxWidth, int maxHeight);

            virtual void SetAutoSize(bool autosize);
            virtual bool AutoSize();
            virtual Base::NSize GetAutoSize() const;
            virtual bool IsAutoSize() const;

            virtual void SetMargin(int left, int top, int right, int bottom);
            virtual const Base::NRect& GetMargin() const;
            virtual void SetPadding(int left, int top, int right, int bottom);
            virtual const Base::NRect& GetPadding() const;
            virtual void SetLayout(UINT layout);
            virtual UINT GetLayout() const;
            virtual void ReLayout();
            virtual void SetLayoutable(bool layoutable);
            virtual bool IsLayoutable() const;

            virtual void OnMouseDown(short x, short y);
            virtual void OnMouseUp();
            virtual void OnMouseMove(short x, short y);
            virtual void OnMouseHover();
            virtual void OnMouseLeave();

            virtual bool OnKeyDown(TCHAR key);
            virtual bool OnKeyUp(TCHAR key);

        protected:
            virtual void OnParentChanged();
            virtual void OnWindowChanged(NWindow* window);

            virtual bool CanHover() const;
            virtual void OnSizeChanged(int width, int height);
            virtual void OnPosChanged(int left, int top);

            // Draw
            virtual void GetDrawIndex(int& horzIndex, int& vertIndex) const;
            virtual void DrawBkg(NRender* render, const Base::NRect& rect) const;
            virtual void DrawFore(NRender* render, const Base::NRect& rect) const;
            virtual void DrawContent(NRender* render, const Base::NRect& rect) const;
            virtual void DrawChilds(NRender* render, Base::NPoint& ptOffset, HRGN clipRgn);

            virtual bool SetPosImpl(int left, int top, bool force);
            virtual bool SetSizeImpl(int width, int height, bool force);

        private:
            static void SetParentHelper(NFrameBase* child, NFrameBase* newParent);
            FrameList::const_iterator GetChildHelper(NFrameBase* child, size_t& zorder) const;

        public:
            // Event
            class SizeEventData : public NEventData
            {
            public:
                int width;
                int height;
            };
            NEvent SizeEvent;

            class KeyEventData : public NEventData
            {
            public:
                TCHAR key;
            };
            NEvent KeyDownEvent;
            NEvent KeyUpEvent;

            NEvent PreDrawEvent;

        protected:
            size_t topMostCount_;
            size_t bottomMostCount_;
            NFrameBase* parentFrame_;
            FrameList childs_;

            Base::NAutoPtr<NWindow> window_;

            // NFrameBase::Flag
            DWORD frameFlags_;

            // NFrameBase::Status
            DWORD frameStatus_;

            Base::NString frameId_;
            Base::NRect frameRect_;
            Base::NSize minSize_;
            Base::NSize maxSize_;
            Base::NRect margin_;
            Base::NRect padding_;
            UINT layout_;
            DWORD frameData_;
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}
