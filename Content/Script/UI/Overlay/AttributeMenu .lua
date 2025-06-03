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

--function M:Construct()
--    self.Btn_CloseAttribute.Button.OnClicked:Add(self,self.OnCloseButtonClicked)
--    self:SetWidgetController(UE.UAuraAbilitySystemLibrary.GetAttributeMenuWidgetController(self))
--    local playerController = UE.UGameplayStatics.GetPlayerController(self,0)
--    ---@type BP_AuraHUD_C
--    local AuraHud = playerController:GetHUD()
--    AuraHud:SetMenuOpen(true)
--end

function M:OnCloseButtonClicked()
    ---@type WBP_ConfirmBox_C
    UE.UUIFrameLibrary.CreateMessageBox(self, function() self:CloseMenu() end, nil, "Are you sure you want to close the attribute menu?", UE.EConfirmBox_Type.ConfirmBox_Type_Normal)
end


function M:CloseMenu()
    self:RemoveFromParent()
end


--function M:Tick(MyGeometry, InDeltaTime)
--end

function M:Destruct()
    self.OnAttributeMenuClosed:Broadcast()
    local playerController = UE.UGameplayStatics.GetPlayerController(self,0)
    ---@type BP_AuraHUD_C
    local AuraHud = playerController:GetHUD()
    AuraHud:SetMenuOpen(false)
end

function M:WidgetControllerSet()
    ---@type BP_AttributeMenuWidgetController_C
    local widgetController = self.WidgetController
    widgetController:BroadcastInitialValues()
end

return M

