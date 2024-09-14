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
    self.Btn_AttributeMenu.Button.OnClicked:Add(self,self.OnAttributeBtnClicked)
end

---@param Row FUIWidgetRow
function M:OnGetMessageWidgetRow(Row)
    ---@type WBP_EffectMessage_C
    local EffectMessageWidget = UE.UWidgetBlueprintLibrary.Create(self,Row.MessageWidget)
    EffectMessageWidget:SetImgAndText(Row.Image,Row.Message)
    local ViewportSize = UE.UWidgetLayoutLibrary.GetViewportSize(self)
    local Position = UE.FVector2D(ViewportSize.X*0.1,ViewportSize.Y*0.8)
    EffectMessageWidget:SetPositionInViewport(Position,true)
    EffectMessageWidget:AddToViewport()
end

function M:OnAttributeBtnClicked()
    self.Btn_AttributeMenu.Button:SetIsEnabled(false)
    ---@type WBP_AttributesMenu_C
    local AttributeMenu = UE.UWidgetBlueprintLibrary.Create(self, UE.UClass.Load("/Game/Blueprints/UI/AttributeMenu/WBP_AttributesMenu.WBP_AttributesMenu_C"))
    local playerController = UE.UGameplayStatics.GetPlayerController(self,0)
    UE.UWidgetBlueprintLibrary.SetInputMode_UIOnlyEx(playerController,nil,UE.EMouseLockMode.DoNotLock,false)
    AttributeMenu:AddToViewport()
    AttributeMenu:SetPositionInViewport(UE.FVector2D(50,50),true)
    AttributeMenu.OnAttributeMenuClosed:Add(self,self.ReEnableAttributeBtn)
end

function M:ReEnableAttributeBtn()
    self.Btn_AttributeMenu.Button:SetIsEnabled(true) 
end

return M
