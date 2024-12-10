--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_EnemyHealthBar_C
local M = UnLua.Class(WBP_ProgressBar_C)

function M:Initialize(Initializer)
    self.Health = 0
    self.MaxHealth = 1
end

--function M:PreConstruct(IsDesignTime)
--end

-- function M:Construct()
-- end

function M:WidgetControllerSet()
    self.WidgetController.OnHealthChanged:Add(self,self.OnHealthChanged)
    self.WidgetController.OnMaxHealthChanged:Add(self,self.OnMaxHealthChanged)
end

function M:OnHealthChanged(NewHealth)
    self.Health = NewHealth
    self:SetBarProgress(self.Health, self.MaxHealth)
end

function M:OnMaxHealthChanged(NewMaxHealth)
    self.MaxHealth = NewMaxHealth
    self:SetBarProgress(self.Health, self.MaxHealth)
end

return M
