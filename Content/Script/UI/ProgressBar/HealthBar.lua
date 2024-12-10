--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_HealthGlobe_C
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
    self.WidgetController.OnHealthChanged:Add(self,self.OnHealthChanged)
    self.WidgetController.OnMaxHealthChanged:Add(self,self.OnMaxHealthChanged)
end

function M:OnHealthChanged(newHealth)
    self.Health = newHealth
    self:UpdateBar(self.Health, self.MaxHealth)
end

function M:OnMaxHealthChanged(newMaxHealth)
    self.MaxHealth = newMaxHealth
    self:UpdateBar(self.Health, self.MaxHealth)
end



return M
