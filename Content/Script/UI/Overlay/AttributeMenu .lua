--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_AttributesMenu_C
local M = UnLua.Class()

--function M:Initialize(Initializer)
--end

--function M:PreConstruct(IsDesignTime)
--end

function M:Construct()
    self.Btn_CloseAttribute.Button.OnClicked:Add(self,self.OnCloseButtonClicked)
    self:SetWidgetController(UE.UAuraAbilitySystemLibrary.GetAttributeMenuWidgetController(self))
end

function M:OnCloseButtonClicked()
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

