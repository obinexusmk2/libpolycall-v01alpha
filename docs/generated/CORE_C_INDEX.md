# Core C Reference Index

## Scope

- Headers scanned: `libpolycall-v1/include/*.h`
- Sources scanned: `libpolycall-v1/src/*.c`

## Exported APIs

### `libpolycall-v1/include/network.h`
- `bool net_init(NetworkEndpoint* endpoint);`
- `bool net_is_port_in_use(uint16_t port);`
- `bool net_release_port(uint16_t port);`
- `ssize_t net_receive(NetworkEndpoint* endpoint, NetworkPacket* packet);`
- `ssize_t net_send(NetworkEndpoint* endpoint, NetworkPacket* packet);`
- `void net_cleanup_client_state(ClientState* state);`
- `void net_cleanup_program(NetworkProgram* program);`
- `void net_close(NetworkEndpoint* endpoint);`
- `void net_init_client_state(ClientState* state);`
- `void net_init_program(NetworkProgram* program);`
- `void net_run(NetworkProgram* program);`

### `libpolycall-v1/include/polycall.h`
- `const char* polycall_get_last_error(polycall_context_t ctx);`
- `const char* polycall_get_version(void);`
- `polycall_status_t polycall_init_with_config(polycall_context_t* ctx, const polycall_config_t* config);`
- `void polycall_cleanup(polycall_context_t ctx);`

### `libpolycall-v1/include/polycall_file_parser.h`
- _(no exported prototypes detected)_

### `libpolycall-v1/include/polycall_micro.h`
- `PolycallMicroStatus polycall_micro_batch_process(PolycallMicroContext* ctx, uint32_t service_id, const PolycallCommandArray* commands);`
- `PolycallMicroStatus polycall_micro_collect_garbage(PolycallMicroContext* ctx);`
- `PolycallMicroStatus polycall_micro_create_service(PolycallMicroContext* ctx, uint32_t service_id, uint32_t flags);`
- `PolycallMicroStatus polycall_micro_destroy_service(PolycallMicroContext* ctx, uint32_t service_id);`
- `PolycallMicroStatus polycall_micro_filter_commands(PolycallCommandArray* commands, PolycallPredicate predicate);`
- `PolycallMicroStatus polycall_micro_init(PolycallMicroContext* ctx, const polycall_config_t* config);`
- `PolycallMicroStatus polycall_micro_process_commands(PolycallServiceState* service, PolycallOperation operation);`
- `PolycallMicroStatus polycall_micro_transform_command(PolycallCommand* cmd, const PolycallTransformChain* chain);`
- `PolycallMicroStatus polycall_micro_update_service_state(PolycallMicroContext* ctx, uint32_t service_id, uint32_t new_state);`
- `PolycallTransformChain polycall_micro_create_transform_chain(PolycallTransform* transforms, uint32_t count);`
- `const char* polycall_micro_status_string(PolycallMicroStatus status);`
- `uint32_t polycall_micro_get_active_services(const PolycallMicroContext* ctx);`
- `void polycall_micro_cleanup(PolycallMicroContext* ctx);`
- `void polycall_micro_destroy_transform_chain(PolycallTransformChain* chain);`

### `libpolycall-v1/include/polycall_parser.h`
- `PolycallAST* polycall_ast_apply_transforms(const PolycallAST* ast, const PolycallASTTransforms* transforms);`
- `PolycallAST* polycall_ast_filter(const PolycallAST* ast, ASTPredicate predicate);`
- `PolycallAST* polycall_ast_optimize(const PolycallAST* ast, uint32_t level);`
- `PolycallAST* polycall_parser_parse_file(PolycallParser* parser, const char* filename);`
- `PolycallAST* polycall_parser_parse_string(PolycallParser* parser, const char* input, size_t length);`
- `PolycallASTNode* polycall_ast_create_node(PolycallASTType type, const PolycallValue* value);`
- `PolycallASTNode* polycall_ast_find_node(const PolycallAST* ast, PolycallASTType type);`
- `PolycallASTNode* polycall_ast_map(const PolycallASTNode* node, ASTTransform transform);`
- `PolycallASTNode** polycall_ast_find_nodes(const PolycallAST* ast, PolycallASTType type, uint32_t* count);`
- `PolycallASTTransforms* polycall_ast_create_transforms(ASTTransform* transforms, uint32_t count);`
- `PolycallParser* polycall_parser_create(const PolycallParserConfig* config);`
- `bool polycall_ast_add_child(PolycallASTNode* parent, PolycallASTNode* child);`
- `bool polycall_ast_validate(const PolycallAST* ast);`
- `const char* polycall_parser_get_error(const PolycallParser* parser);`
- `void polycall_ast_destroy_node(PolycallASTNode* node);`
- `void polycall_ast_destroy_transforms(PolycallASTTransforms* transforms);`
- `void polycall_ast_visit(PolycallASTNode* node, ASTVisitor visitor, void* user_data);`
- `void polycall_parser_destroy(PolycallParser* parser);`

### `libpolycall-v1/include/polycall_protocol.h`
- `bool polycall_protocol_authenticate(polycall_protocol_context_t* ctx, const char* credentials, size_t credentials_length);`
- `bool polycall_protocol_can_transition(const polycall_protocol_context_t* ctx, polycall_protocol_state_t target_state);`
- `bool polycall_protocol_complete_handshake(polycall_protocol_context_t* ctx);`
- `bool polycall_protocol_init(polycall_protocol_context_t* ctx, polycall_context_t pc_ctx, NetworkEndpoint* endpoint, const polycall_protocol_config_t* config);`
- `bool polycall_protocol_is_authenticated(const polycall_protocol_context_t* ctx);`
- `bool polycall_protocol_is_connected(const polycall_protocol_context_t* ctx);`
- `bool polycall_protocol_is_error(const polycall_protocol_context_t* ctx);`
- `bool polycall_protocol_process(polycall_protocol_context_t* ctx, const void* data, size_t length);`
- `bool polycall_protocol_send(polycall_protocol_context_t* ctx, polycall_message_type_t type, const void* payload, size_t payload_length, polycall_protocol_flags_t flags);`
- `bool polycall_protocol_start_handshake(polycall_protocol_context_t* ctx);`
- `bool polycall_protocol_verify_checksum(const polycall_message_header_t* header, const void* payload, size_t payload_length);`
- `bool polycall_protocol_version_compatible(uint8_t remote_version);`
- `const char* polycall_protocol_get_error(const polycall_protocol_context_t* ctx);`
- `polycall_message_header_t polycall_protocol_create_header(polycall_message_type_t type, size_t payload_length, polycall_protocol_flags_t flags);`
- `polycall_protocol_state_t polycall_protocol_get_state(const polycall_protocol_context_t* ctx);`
- `uint32_t polycall_protocol_calculate_checksum(const void* data, size_t length);`
- `void polycall_protocol_cleanup(polycall_protocol_context_t* ctx);`
- `void polycall_protocol_set_error(polycall_protocol_context_t* ctx, const char* error);`
- `void polycall_protocol_update(polycall_protocol_context_t* ctx);`

### `libpolycall-v1/include/polycall_state_machine.h`
- `polycall_sm_status_t polycall_sm_add_state(PolyCall_StateMachine* sm, const char* name, PolyCall_StateAction on_enter, PolyCall_StateAction on_exit, bool is_final);`
- `polycall_sm_status_t polycall_sm_add_transition(PolyCall_StateMachine* sm, const char* name, unsigned int from_state, unsigned int to_state, PolyCall_StateAction action, bool (*guard_condition)(const PolyCall_State*, const PolyCall_State*));`
- `polycall_sm_status_t polycall_sm_create_state_snapshot(const PolyCall_StateMachine* sm, unsigned int state_id, PolyCall_StateSnapshot* snapshot);`
- `polycall_sm_status_t polycall_sm_create_with_integrity(polycall_context_t ctx, PolyCall_StateMachine** sm, PolyCall_StateIntegrityCheck integrity_check);`
- `polycall_sm_status_t polycall_sm_execute_transition(PolyCall_StateMachine* sm, const char* transition_name);`
- `polycall_sm_status_t polycall_sm_get_state_diagnostics(const PolyCall_StateMachine* sm, unsigned int state_id, PolyCall_StateDiagnostics* diagnostics);`
- `polycall_sm_status_t polycall_sm_get_state_version(const PolyCall_StateMachine* sm, unsigned int state_id, unsigned int* version);`
- `polycall_sm_status_t polycall_sm_lock_state(PolyCall_StateMachine* sm, unsigned int state_id);`
- `polycall_sm_status_t polycall_sm_restore_state_from_snapshot(PolyCall_StateMachine* sm, const PolyCall_StateSnapshot* snapshot);`
- `polycall_sm_status_t polycall_sm_unlock_state(PolyCall_StateMachine* sm, unsigned int state_id);`
- `polycall_sm_status_t polycall_sm_verify_state_integrity(PolyCall_StateMachine* sm, unsigned int state_id);`
- `void polycall_sm_destroy(PolyCall_StateMachine* sm);`

### `libpolycall-v1/include/polycall_token.h`
- `PolycallToken polycall_token_map(const PolycallToken* token, TokenOperation op);`
- `PolycallTokenArray* polycall_token_chain(const PolycallTokenArray* array, const PolycallTokenOperations* ops);`
- `PolycallTokenArray* polycall_token_create_array(uint32_t capacity);`
- `PolycallTokenArray* polycall_token_filter(const PolycallTokenArray* array, TokenPredicate pred);`
- `PolycallValue polycall_value_create(PolycallValueType type, const void* data);`
- `bool polycall_value_equals(const PolycallValue* a, const PolycallValue* b);`
- `void polycall_token_destroy_array(PolycallTokenArray* array);`
- `void polycall_value_destroy(PolycallValue* value);`

### `libpolycall-v1/include/polycall_tokenizer.h`
- `POLYCALL_API PolycallTokenizer* polycall_tokenizer_create(const PolycallTokenizerConfig* config);`
- `POLYCALL_API PolycallTokenizerState polycall_tokenizer_get_state(const PolycallTokenizer* tokenizer);`
- `POLYCALL_API TokenizerOperations* polycall_tokenizer_compose_ops(const TokenizerOperations* ops1, const TokenizerOperations* ops2);`
- `POLYCALL_API TokenizerOperations* polycall_tokenizer_create_ops(TokenPattern* patterns, TokenConsumer* consumers, size_t count);`
- `POLYCALL_API bool polycall_tokenizer_match_identifier(const char* input, size_t* length);`
- `POLYCALL_API bool polycall_tokenizer_match_number(const char* input, size_t* length);`
- `POLYCALL_API bool polycall_tokenizer_match_operator(const char* input, size_t* length);`
- `POLYCALL_API bool polycall_tokenizer_match_string(const char* input, size_t* length);`
- `POLYCALL_API bool polycall_tokenizer_process(PolycallTokenizer* tokenizer, const TokenizerOperations* ops);`
- `POLYCALL_API bool polycall_tokenizer_set_input(PolycallTokenizer* tokenizer, const char* input, size_t length);`
- `POLYCALL_API const PolycallTokenArray* polycall_tokenizer_get_tokens(const PolycallTokenizer* tokenizer);`
- `POLYCALL_API const char* polycall_tokenizer_get_error(const PolycallTokenizer* tokenizer);`
- `POLYCALL_API void polycall_tokenizer_destroy(PolycallTokenizer* tokenizer);`
- `POLYCALL_API void polycall_tokenizer_destroy_ops(TokenizerOperations* ops);`
- `POLYCALL_API void polycall_tokenizer_reset(PolycallTokenizer* tokenizer);`

## Key Structs

### `libpolycall-v1/include/network.h`
- `ClientState`
- `NetworkEndpoint`
- `NetworkPacket`
- `handlers`

### `libpolycall-v1/include/polycall.h`
- `polycall_config_t`

### `libpolycall-v1/include/polycall_micro.h`
- `PolycallCommand`
- `PolycallCommandArray`
- `PolycallMicroContext`
- `PolycallServiceArray`
- `PolycallServiceState`
- `PolycallTransformChain`

### `libpolycall-v1/include/polycall_parser.h`
- `PolycallAST`
- `PolycallASTNode`
- `PolycallASTTransforms`
- `PolycallParserConfig`
- `error`

### `libpolycall-v1/include/polycall_protocol.h`
- `polycall_message_header_t`
- `polycall_protocol_callbacks_t`
- `polycall_protocol_config_t`
- `polycall_protocol_context_t`

### `libpolycall-v1/include/polycall_state_machine.h`
- `PolyCall_State`
- `PolyCall_StateDiagnostics`
- `PolyCall_StateSnapshot`
- `PolyCall_Transition`
- `diagnostics`

### `libpolycall-v1/include/polycall_token.h`
- `PolycallToken`
- `PolycallTokenOperations`
- `position`
- `string_value`

### `libpolycall-v1/include/polycall_tokenizer.h`
- `TokenConsumer`
- `TokenPattern`
- `TokenizerOperations`
- `input`
- `limits`

## File-Level Responsibility

- `libpolycall-v1/src/network.c`: Implements functions: find_available_port, net_add_client, net_cleanup_client_state, net_cleanup_program, net_close, +10 more.
- `libpolycall-v1/src/polycall.c`: Implements functions: polycall_cleanup, polycall_get_last_error, polycall_get_version, polycall_init_with_config, set_error.
- `libpolycall-v1/src/polycall_micro.c`: Implements functions: get_current_timestamp, polycall_micro_batch_process, polycall_micro_cleanup, polycall_micro_collect_garbage, polycall_micro_create_service, +12 more.
- `libpolycall-v1/src/polycall_parser.c`: Implements functions: advance_token, allocate_node, collect_optimized_node, count_node, create_node_pool, +32 more.
- `libpolycall-v1/src/polycall_protocol.c`: Implements functions: polycall_protocol_authenticate, polycall_protocol_calculate_checksum, polycall_protocol_can_transition, polycall_protocol_cleanup, polycall_protocol_complete_handshake, +15 more.
- `libpolycall-v1/src/polycall_state_machine.c`: Implements functions: calculate_state_checksum, init_default_transitions, init_state, initialize_state_machine, on_error, +17 more.
- `libpolycall-v1/src/polycall_token.c`: Implements polycall token logic.
- `libpolycall-v1/src/polycall_tokenizer.c`: Implements functions: create_token, is_identifier_part, is_identifier_start, polycall_strdup, polycall_tokenizer_compose_ops, +16 more.
