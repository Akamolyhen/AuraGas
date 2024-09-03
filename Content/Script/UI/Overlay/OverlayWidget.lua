--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_Overlay_C
local M = UnLua.Class()

--function M:Initialize(Initializer)
--end

--function M:PreConstruct(IsDesignTime)
--end

-- function M:Construct()
-- end

--function M:Tick(MyGeometry, InDeltaTime)
--end

function M:WidgetControllerSet()
    ---@type BP_OverlayWidgetController_C
    local WidgetController = self.WidgetController
    WidgetController.MessageWidgetRowDelegate:Add(self,self.OnGetMessageWidgetRow)
    self.WBP_HealthGlobe:SetWidgetController(self.WidgetController)
    self.WBP_ManaGlobe:SetWidgetController(self.WidgetController)
end

---@param Row FUIWidgetRow
function M:OnGetMessageWidgetRow(Row)
    ---@type WBP_EffectMessage_C
    local EffectMessageWidget = UE.UWidgetBlueprintLibrary.Create(self,Row.MessageWidget)
    EffectMessageWidget:SetImgAndText(Row.Image,Row.Message)
    local ViewportSize = UE.UWidgetLayoutLibrary.GetViewportSize(self)
    local Position = UE.FVector2D(ViewportSize.X*0.1,ViewportSize.Y*0.6)
    EffectMessageWidget:SetPositionInViewport(Position,true)
    EffectMessageWidget:AddToViewport()
end

return M
