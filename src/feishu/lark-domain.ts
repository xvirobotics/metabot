import * as lark from '@larksuiteoapi/node-sdk';

export type FeishuDomain = 'feishu' | 'lark';

/** Map a configured Feishu/Lark domain to the SDK domain; defaults to Feishu (China). */
export function toLarkSdkDomain(domain: FeishuDomain | undefined): lark.Domain {
  return domain === 'lark' ? lark.Domain.Lark : lark.Domain.Feishu;
}
