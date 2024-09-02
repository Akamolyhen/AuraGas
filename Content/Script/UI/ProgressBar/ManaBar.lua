--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_ManaGlobe_C
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
    WidgetController.OnManaChanged:Add(self,self.OnManaChanged)
    WidgetController.OnMaxManaChanged:Add(self,self.OnMaxManaChanged)
end

function M:OnManaChanged(newMana)
    self.Mana = newMana
    self:SetProgressBarPercent(UE.UKismetMathLibrary.SafeDivide(self.Mana, self.MaxMana))
end

function M:OnMaxManaChanged(newMaxMana)
    self.MaxMana = newMaxMana
    self:SetProgressBarPercent(UE.UKismetMathLibrary.SafeDivide(self.Mana, self.MaxMana))
end
return M
