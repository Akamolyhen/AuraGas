--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_AttributesMenu_C
local M = UnLua.Class()

function M:Initialize(Initializer)
end

--function M:PreConstruct(IsDesignTime)
--end

function M:Construct()
    self.Btn_CloseAttribute.Button.OnClicked:Add(self,self.OnCloseButtonClicked)
    self:SetWidgetController(UE.UAuraAbilitySystemLibrary.GetAttributeMenuWidgetController(self))
end

function M:OnCloseButtonClicked()
    local playerController = UE.UGameplayStatics.GetPlayerController(self,0)
    ---@type BP_AuraHUD_C
    local AuraHud = playerController:GetHUD()
    ---@type WBP_ConfirmBox_C
    local ConfirmBox = AuraHud:ShowConfirmBox("Are you sure you want to close the attribute menu?")
    ConfirmBox.EventDispatcher_OnConfirmEvent:Add(self,self.CloseMenu)
    ConfirmBox.EventDispatcher_OnCancelEvent:Add(self,self.CloseConfirmBox)
end

function M:CloseConfirmBox()
    local playerController = UE.UGameplayStatics.GetPlayerController(self,0)
    ---@type BP_AuraHUD_C
    local AuraHud = playerController:GetHUD() 
    AuraHud:RemoveConfirmBox()
end

function M:CloseMenu()
    self:CloseConfirmBox()
    self:RemoveFromParent()
    local playerController = UE.UGameplayStatics.GetPlayerController(self,0)
    UE.UWidgetBlueprintLibrary.SetInputMode_GameAndUIEx(playerController,nil,UE.EMouseLockMode.DoNotLock,false,false)
end


--function M:Tick(MyGeometry, InDeltaTime)
--end

function M:Destruct()
    self.OnAttributeMenuClosed:Broadcast()
end

function M:WidgetControllerSet()
    ---@type BP_AttributeMenuWidgetController_C
    local widgetController = self.WidgetController
    widgetController:BroadcastInitialValues()
end

return M

