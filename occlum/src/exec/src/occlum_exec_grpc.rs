// This file is generated. Do not edit
// @generated

// https://github.com/Manishearth/rust-clippy/issues/702
#![allow(unknown_lints)]
#![allow(clippy::all)]

#![cfg_attr(rustfmt, rustfmt_skip)]

#![allow(box_pointers)]
#![allow(dead_code)]
#![allow(missing_docs)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
#![allow(non_upper_case_globals)]
#![allow(trivial_casts)]
#![allow(unsafe_code)]
#![allow(unused_imports)]
#![allow(unused_results)]


// server interface

pub trait OcclumExec {
    fn status_check(&self, o: ::grpc::ServerHandlerContext, req: ::grpc::ServerRequestSingle<super::occlum_exec::HealthCheckRequest>, resp: ::grpc::ServerResponseUnarySink<super::occlum_exec::HealthCheckResponse>) -> ::grpc::Result<()>;

    fn exec_command(&self, o: ::grpc::ServerHandlerContext, req: ::grpc::ServerRequestSingle<super::occlum_exec::ExecCommRequest>, resp: ::grpc::ServerResponseUnarySink<super::occlum_exec::ExecCommResponse>) -> ::grpc::Result<()>;

    fn get_result(&self, o: ::grpc::ServerHandlerContext, req: ::grpc::ServerRequestSingle<super::occlum_exec::GetResultRequest>, resp: ::grpc::ServerResponseUnarySink<super::occlum_exec::GetResultResponse>) -> ::grpc::Result<()>;

    fn stop_server(&self, o: ::grpc::ServerHandlerContext, req: ::grpc::ServerRequestSingle<super::occlum_exec::StopRequest>, resp: ::grpc::ServerResponseUnarySink<super::occlum_exec::StopResponse>) -> ::grpc::Result<()>;

    fn kill_process(&self, o: ::grpc::ServerHandlerContext, req: ::grpc::ServerRequestSingle<super::occlum_exec::KillProcessRequest>, resp: ::grpc::ServerResponseUnarySink<super::occlum_exec::KillProcessResponse>) -> ::grpc::Result<()>;
}

// client

pub struct OcclumExecClient {
    grpc_client: ::std::sync::Arc<::grpc::Client>,
}

impl ::grpc::ClientStub for OcclumExecClient {
    fn with_client(grpc_client: ::std::sync::Arc<::grpc::Client>) -> Self {
        OcclumExecClient {
            grpc_client: grpc_client,
        }
    }
}

impl OcclumExecClient {
    pub fn status_check(&self, o: ::grpc::RequestOptions, req: super::occlum_exec::HealthCheckRequest) -> ::grpc::SingleResponse<super::occlum_exec::HealthCheckResponse> {
        let descriptor = ::grpc::rt::ArcOrStatic::Static(&::grpc::rt::MethodDescriptor {
            name: ::grpc::rt::StringOrStatic::Static("/occlumexec.OcclumExec/StatusCheck"),
            streaming: ::grpc::rt::GrpcStreaming::Unary,
            req_marshaller: ::grpc::rt::ArcOrStatic::Static(&::grpc_protobuf::MarshallerProtobuf),
            resp_marshaller: ::grpc::rt::ArcOrStatic::Static(&::grpc_protobuf::MarshallerProtobuf),
        });
        self.grpc_client.call_unary(o, req, descriptor)
    }

    pub fn exec_command(&self, o: ::grpc::RequestOptions, req: super::occlum_exec::ExecCommRequest) -> ::grpc::SingleResponse<super::occlum_exec::ExecCommResponse> {
        let descriptor = ::grpc::rt::ArcOrStatic::Static(&::grpc::rt::MethodDescriptor {
            name: ::grpc::rt::StringOrStatic::Static("/occlumexec.OcclumExec/ExecCommand"),
            streaming: ::grpc::rt::GrpcStreaming::Unary,
            req_marshaller: ::grpc::rt::ArcOrStatic::Static(&::grpc_protobuf::MarshallerProtobuf),
            resp_marshaller: ::grpc::rt::ArcOrStatic::Static(&::grpc_protobuf::MarshallerProtobuf),
        });
        self.grpc_client.call_unary(o, req, descriptor)
    }

    pub fn get_result(&self, o: ::grpc::RequestOptions, req: super::occlum_exec::GetResultRequest) -> ::grpc::SingleResponse<super::occlum_exec::GetResultResponse> {
        let descriptor = ::grpc::rt::ArcOrStatic::Static(&::grpc::rt::MethodDescriptor {
            name: ::grpc::rt::StringOrStatic::Static("/occlumexec.OcclumExec/GetResult"),
            streaming: ::grpc::rt::GrpcStreaming::Unary,
            req_marshaller: ::grpc::rt::ArcOrStatic::Static(&::grpc_protobuf::MarshallerProtobuf),
            resp_marshaller: ::grpc::rt::ArcOrStatic::Static(&::grpc_protobuf::MarshallerProtobuf),
        });
        self.grpc_client.call_unary(o, req, descriptor)
    }

    pub fn stop_server(&self, o: ::grpc::RequestOptions, req: super::occlum_exec::StopRequest) -> ::grpc::SingleResponse<super::occlum_exec::StopResponse> {
        let descriptor = ::grpc::rt::ArcOrStatic::Static(&::grpc::rt::MethodDescriptor {
            name: ::grpc::rt::StringOrStatic::Static("/occlumexec.OcclumExec/StopServer"),
            streaming: ::grpc::rt::GrpcStreaming::Unary,
            req_marshaller: ::grpc::rt::ArcOrStatic::Static(&::grpc_protobuf::MarshallerProtobuf),
            resp_marshaller: ::grpc::rt::ArcOrStatic::Static(&::grpc_protobuf::MarshallerProtobuf),
        });
        self.grpc_client.call_unary(o, req, descriptor)
    }

    pub fn kill_process(&self, o: ::grpc::RequestOptions, req: super::occlum_exec::KillProcessRequest) -> ::grpc::SingleResponse<super::occlum_exec::KillProcessResponse> {
        let descriptor = ::grpc::rt::ArcOrStatic::Static(&::grpc::rt::MethodDescriptor {
            name: ::grpc::rt::StringOrStatic::Static("/occlumexec.OcclumExec/KillProcess"),
            streaming: ::grpc::rt::GrpcStreaming::Unary,
            req_marshaller: ::grpc::rt::ArcOrStatic::Static(&::grpc_protobuf::MarshallerProtobuf),
            resp_marshaller: ::grpc::rt::ArcOrStatic::Static(&::grpc_protobuf::MarshallerProtobuf),
        });
        self.grpc_client.call_unary(o, req, descriptor)
    }
}

// server

pub struct OcclumExecServer;


impl OcclumExecServer {
    pub fn new_service_def<H : OcclumExec + 'static + Sync + Send + 'static>(handler: H) -> ::grpc::rt::ServerServiceDefinition {
        let handler_arc = ::std::sync::Arc::new(handler);
        ::grpc::rt::ServerServiceDefinition::new("/occlumexec.OcclumExec",
            vec![
                ::grpc::rt::ServerMethod::new(
                    ::grpc::rt::ArcOrStatic::Static(&::grpc::rt::MethodDescriptor {
                        name: ::grpc::rt::StringOrStatic::Static("/occlumexec.OcclumExec/StatusCheck"),
                        streaming: ::grpc::rt::GrpcStreaming::Unary,
                        req_marshaller: ::grpc::rt::ArcOrStatic::Static(&::grpc_protobuf::MarshallerProtobuf),
                        resp_marshaller: ::grpc::rt::ArcOrStatic::Static(&::grpc_protobuf::MarshallerProtobuf),
                    }),
                    {
                        let handler_copy = handler_arc.clone();
                        ::grpc::rt::MethodHandlerUnary::new(move |ctx, req, resp| (*handler_copy).status_check(ctx, req, resp))
                    },
                ),
                ::grpc::rt::ServerMethod::new(
                    ::grpc::rt::ArcOrStatic::Static(&::grpc::rt::MethodDescriptor {
                        name: ::grpc::rt::StringOrStatic::Static("/occlumexec.OcclumExec/ExecCommand"),
                        streaming: ::grpc::rt::GrpcStreaming::Unary,
                        req_marshaller: ::grpc::rt::ArcOrStatic::Static(&::grpc_protobuf::MarshallerProtobuf),
                        resp_marshaller: ::grpc::rt::ArcOrStatic::Static(&::grpc_protobuf::MarshallerProtobuf),
                    }),
                    {
                        let handler_copy = handler_arc.clone();
                        ::grpc::rt::MethodHandlerUnary::new(move |ctx, req, resp| (*handler_copy).exec_command(ctx, req, resp))
                    },
                ),
                ::grpc::rt::ServerMethod::new(
                    ::grpc::rt::ArcOrStatic::Static(&::grpc::rt::MethodDescriptor {
                        name: ::grpc::rt::StringOrStatic::Static("/occlumexec.OcclumExec/GetResult"),
                        streaming: ::grpc::rt::GrpcStreaming::Unary,
                        req_marshaller: ::grpc::rt::ArcOrStatic::Static(&::grpc_protobuf::MarshallerProtobuf),
                        resp_marshaller: ::grpc::rt::ArcOrStatic::Static(&::grpc_protobuf::MarshallerProtobuf),
                    }),
                    {
                        let handler_copy = handler_arc.clone();
                        ::grpc::rt::MethodHandlerUnary::new(move |ctx, req, resp| (*handler_copy).get_result(ctx, req, resp))
                    },
                ),
                ::grpc::rt::ServerMethod::new(
                    ::grpc::rt::ArcOrStatic::Static(&::grpc::rt::MethodDescriptor {
                        name: ::grpc::rt::StringOrStatic::Static("/occlumexec.OcclumExec/StopServer"),
                        streaming: ::grpc::rt::GrpcStreaming::Unary,
                        req_marshaller: ::grpc::rt::ArcOrStatic::Static(&::grpc_protobuf::MarshallerProtobuf),
                        resp_marshaller: ::grpc::rt::ArcOrStatic::Static(&::grpc_protobuf::MarshallerProtobuf),
                    }),
                    {
                        let handler_copy = handler_arc.clone();
                        ::grpc::rt::MethodHandlerUnary::new(move |ctx, req, resp| (*handler_copy).stop_server(ctx, req, resp))
                    },
                ),
                ::grpc::rt::ServerMethod::new(
                    ::grpc::rt::ArcOrStatic::Static(&::grpc::rt::MethodDescriptor {
                        name: ::grpc::rt::StringOrStatic::Static("/occlumexec.OcclumExec/KillProcess"),
                        streaming: ::grpc::rt::GrpcStreaming::Unary,
                        req_marshaller: ::grpc::rt::ArcOrStatic::Static(&::grpc_protobuf::MarshallerProtobuf),
                        resp_marshaller: ::grpc::rt::ArcOrStatic::Static(&::grpc_protobuf::MarshallerProtobuf),
                    }),
                    {
                        let handler_copy = handler_arc.clone();
                        ::grpc::rt::MethodHandlerUnary::new(move |ctx, req, resp| (*handler_copy).kill_process(ctx, req, resp))
                    },
                ),
            ],
        )
    }
}
