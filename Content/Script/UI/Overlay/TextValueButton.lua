--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_TextValueButton_C
local M = UnLua.Class()

--function M:Initialize(Initializer)
--end

--function M:PreConstruct(IsDesignTime)
--end

function M:Construct()
    ---@type BP_AttributeMenuWidgetController_C
    local widgetController = UE.UAuraAbilitySystemLibrary.GetAttributeMenuWidgetController(self)
    widgetController.AttributeInfoDelegate:Add(self,self.OnAttributeInfoBroad)
end

---@param Info FAuraAttributeInfo
function M:OnAttributeInfoBroad(Info)
    self.LabelName = Info.AttributeName
    self.FramedValue.AttributeVal = Info.AttributeValue
end

--function M:Tick(MyGeometry, InDeltaTime)
--end

return M
