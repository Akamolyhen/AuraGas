--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_Damage_C
local M = UnLua.Class()

--function M:Initialize(Initializer)
--end

--function M:PreConstruct(IsDesignTime)
--end

function M:Construct()
    self:PlayAnimation(self.PopInAndOut,0,1,UE.EUMGSequencePlayMode.Forward,1,false)
end

---@param NewDamage number
function M:UpdateDamageText(NewDamage)
    local text = UE.UKismetTextLibrary.Conv_DoubleToText(NewDamage,0,false,true,1,324,0,0)
    self.Text_Damage:SetText(text)
end

--function M:Tick(MyGeometry, InDeltaTime)
--end

return M
