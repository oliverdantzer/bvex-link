from typing import Callable

import click
from click_shell import shell  # type: ignore
from .sync_metric_ids import MetricInfo


class CLI:
    def __init__(
        self,
        handle_set_bps: Callable[[str, int], bool],
        handle_set_max_bps: Callable[[int], str],
        get_metric_info: Callable[[], set[MetricInfo]],
    ):
        self.handle_set_bps = handle_set_bps
        self.handle_set_max_bps = handle_set_max_bps
        self.get_metric_info = get_metric_info
        self.app = self.create_app()

    def create_app(self):
        @shell(prompt="bvex-cli > ", intro="Bvex cli...")
        def app():
            pass

        @app.command()
        @click.argument("metric_id", type=str)
        @click.argument("bits_per_second", type=int)
        def setbps(metric_id: str, bits_per_second: int):
            success = self.handle_set_bps(metric_id, bits_per_second)
            if not success:
                click.echo(f"Failed to set bps for {metric_id}")
        
        @app.command()
        @click.argument("bits_per_second", type=int)
        def setmaxbps(bits_per_second: int):
            click.echo(self.handle_set_max_bps(bits_per_second))

        @app.command()
        def getmetricinfo():
            click.echo(self.get_metric_info())

        return app

    def run(self):
        self.app()
