import asyncio
from bvex_codec.telecommand import Subscribe, Telecommand
from bvex_codec.telemetry import Telemetry, WhichTMMessageType
from bvex_codec.sample import Sample, WhichDataType, PrimitiveData
from typing import Callable
from pydantic import ValidationError
from typing import Awaitable


async def subscribe(
    remote_addr: tuple[str, int], metric_id: str, bps: int, handle_sample: Callable[[Sample], Awaitable[None]]
):
    reader, writer = await asyncio.open_connection(remote_addr[0], remote_addr[1])
    try:
        cmd = Subscribe(metric_id=metric_id, bps=bps)
        tc = Telecommand.from_command(cmd)
        writer.write(tc.model_dump_json().encode())
        await writer.drain()

        while True:
            data = await reader.read(4096)
            if data:
                try:
                    telemetry = Telemetry.model_validate_json(data.decode())
                except ValidationError as e:
                    # packet likely was corrupted
                    print(e)
                    continue
                if isinstance(telemetry.data, Sample):
                    await handle_sample(telemetry.data)
            else:
                await asyncio.sleep(0.1)
    except Exception as e:
        print(f"Error: {e}")
        raise e
    finally:
        writer.close()
        await writer.wait_closed()


# async def subscribe_all(
#     remote_addr: tuple[str, int],
#     metric_id_store: MetricIdsStore,
#     handle_sample: Callable[[Sample], Awaitable[None]],
# ):
#     subscriptions: dict[str, asyncio.Task] = {}

#     def get_subscribed_metric_ids():
#         return set(subscriptions.keys())

#     async def cancel_subscription(metric_id: str):
#         if metric_id in subscriptions:
#             subscriptions[metric_id].cancel()
#             try:
#                 await subscriptions[metric_id]
#             except asyncio.CancelledError:
#                 pass
#             del subscriptions[metric_id]

#     async def cancel_subscriptions(metric_ids: set[str]):
#         async with asyncio.TaskGroup() as tg:
#             for metric_id in metric_ids:
#                 tg.create_task(cancel_subscription(metric_id))

#     async def add_subscription(metric_info: MetricInfo):
#         if metric_info.metric_id not in subscriptions:
#             subscriptions[metric_info.metric_id] = asyncio.create_task(
#                 subscribe(remote_addr, metric_info.metric_id, metric_info.bps, handle_sample)
#             )
    
#     async def re_subscribe(metric_info: MetricInfo):
#         if metric_info.metric_id in subscriptions:
#             await cancel_subscription(metric_info.metric_id)
#         await add_subscription(metric_info)

#     try:
#         while True: 
#             await metric_id_store.updated.wait()
#             new_metrics = metric_id_store.get_updated_metrics()
#             subscribed_metric_ids = get_subscribed_metric_ids()
#             async with asyncio.TaskGroup() as tg:
#                 for new_metric in new_metrics:
#                     # if metric id is not subscribed, add it to subscriptions
#                     if new_metric.metric_id not in subscribed_metric_ids:
#                         tg.create_task(add_subscription(new_metric))
#                     # if metric info changed, cancel the subscription and add it again
#                     else:
#                         tg.create_task(re_subscribe(new_metric))
            

#     finally:
#         await cancel_subscriptions(set(subscriptions.keys()))
