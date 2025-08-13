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
    self.Btn_CloseAttribute.Button.OnClicked:Add(self,self.CloseMenu)
    self:SetWidgetController(UE.UAuraAbilitySystemLibrary.GetAttributeMenuWidgetController(self))
    local playerController = UE.UGameplayStatics.GetPlayerController(self,0)
    ---@type BP_AuraHUD_C
    local AuraHud = playerController:GetHUD()
    AuraHud:SetMenuOpen(true)
    --UE.UUIFrameLibrary.CreateMessageBox(self,function() end, function() end,"是否关闭属性菜单")
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

--对Attribute数据进行初始化
function M:WidgetControllerSet()
    self:BindInitAttributes()
    ---@type BP_AttributeMenuWidgetController_C
    local widgetController = self.WidgetController
    widgetController:BroadcastInitialValues()
end

return M

