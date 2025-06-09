from typing import Callable

import click
from click_shell import shell  # type: ignore
from .sync_metric_ids import MetricInfo


class CLI:
    def __init__(
        self,
        handle_set_bps: Callable[[str, int], None],
        handle_set_max_bps: Callable[[int], None],
        get_metric_infos: Callable[[], set[MetricInfo]]
    ):
        self.handle_set_bps = handle_set_bps
        self.handle_set_max_bps = handle_set_max_bps
        self.get_metric_infos = get_metric_infos
        self.app = self.create_app()

    def create_app(self):
        @shell(prompt="bvex-cli > ", intro="Bvex cli...")
        def app():
            pass

        @app.command()
        @click.argument("metric_id", type=str)
        @click.argument("bits_per_second", type=int)
        def setbps(metric_id: str, bits_per_second: int):
            self.handle_set_bps(metric_id, bits_per_second)
        
        @app.command()
        @click.argument("bits_per_second", type=int)
        def setmaxbps(bits_per_second: int):
            click.echo(self.handle_set_max_bps(bits_per_second))

        @app.command()
        def getmetricinfos():
            metric_infos = self.get_metric_infos()
            if not metric_infos:
                click.echo("No metric infos recieved from onboard server.")
            for metric_info in metric_infos:
                click.echo(str(metric_info))

        return app

    def run(self):
        self.app()
