/**
 * Build the small, engine-local appendix Claude Code needs to operate.
 *
 * Privacy boundary: this helper deliberately does not accept ApiContext.
 * Bot names, platform chat/session identifiers, group membership, Team state,
 * and MetaBot commands are bridge-local metadata and must not be forwarded to
 * Claude's system prompt.
 */
export function buildClaudeSystemPromptAppend(options: {
  outputsDirAvailable?: boolean;
  persistent?: boolean;
}): string | undefined {
  const sections: string[] = [];

  if (options.outputsDirAvailable) {
    sections.push(
      '## Output Files\nWhen producing output files for the user (images, PDFs, documents, archives, code files, etc.), copy them to the directory in `$ASSISTANT_OUTPUTS_DIR`. Use `cp` via the Bash tool. Files placed there will be delivered to the user. Do not print or resolve the directory path.',
    );
  }

  if (options.persistent) {
    sections.push(
      [
        '## Persistent Session',
        'You are running inside a LONG-LIVED Claude Code session: this same process serves all the user\'s turns in this conversation. Teammates, /goal multi-turn loops, and background tasks survive across user messages. The session is only torn down on reset or after 30 minutes of total silence.',
      ].join('\n'),
    );
  }

  return sections.length > 0 ? '\n\n' + sections.join('\n\n') : undefined;
}
