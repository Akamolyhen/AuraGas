--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--
local Util = require("Util")
---@type ABP_Enemy_C
local M = UnLua.Class()

-- function M:Initialize(Initializer)
-- end

function M:BlueprintInitializeAnimation() 
    local Enemy = self:TryGetPawnOwner()
    if Enemy then
        self.CharacterMovement = Enemy:GetMovementComponent() 
    end
end

-- function M:BlueprintBeginPlay()
-- end

function M:BlueprintUpdateAnimation(DeltaTimeX)
    if self.CharacterMovement then
        self.GroundSpeed = UE.UKismetMathLibrary.VSizeXY(self.CharacterMovement.Velocity) 
    end
end

-- function M:BlueprintPostEvaluateAnimation()
-- end

return M
