#include "attackInstance.h"

AttackInstance::AttackInstance(const AttackComponent& instigator, const AttackTargetComponent& target)
	: instigatorTransform(instigator.get_global_transform())
	, targetTransform(target.get_global_transform())
	, attackStrength(instigator.getAttackStrength()) {}
