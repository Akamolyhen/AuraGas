--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_TextValueRow_C
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
    if self.AttributeTag and UE.UBlueprintGameplayTagLibrary.MatchesTag(Info.AttributeTag,self.AttributeTag,true) then
        self.LabelName = Info.AttributeName
        self.FramedValue.AttributeVal = math.floor(Info.AttributeValue + 0.5)
    end
end

--function M:Tick(MyGeometry, InDeltaTime)
--end

return M
