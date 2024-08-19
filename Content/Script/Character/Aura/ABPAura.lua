--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type ABP_Aura_C
local M = UnLua.Class()

-- function M:Initialize(Initializer)
-- end

function M:BlueprintInitializeAnimation()
    ---@type BP_AuraCharacter_C
    self.Aura_Character = self:TryGetPawnOwner()
    if self.Aura_Character then
        self.CharacterMovement = self.Aura_Character:GetMovementComponent() 
    end
end

-- function M:BlueprintBeginPlay()
-- end

function M:BlueprintUpdateAnimation(DeltaTimeX)
    self.ShouldMove = false
    if self.Aura_Character and self.CharacterMovement then
        self.GroundSpeed = UE.UKismetMathLibrary.VSizeXY(self.CharacterMovement.Velocity)
        if self.GroundSpeed > 30 then
            self.ShouldMove = true
        end
    end
end

-- function M:BlueprintPostEvaluateAnimation()
-- end

return M
