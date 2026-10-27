/** Endpoint default leaves provider-native settings and environment in control. */
export type ModelPolicy =
  | { readonly kind: 'named'; readonly name: string }
  | { readonly kind: 'endpoint-default' };

export function resolveModelPolicy(
  policy: ModelPolicy | undefined,
  requested: string | undefined,
  configured?: string,
): ModelPolicy {
  if (policy) return policy;
  const name = requested ?? configured;
  return name ? { kind: 'named', name } : { kind: 'endpoint-default' };
}

export function modelName(policy: ModelPolicy): string | undefined {
  return policy.kind === 'named' ? policy.name : undefined;
}
