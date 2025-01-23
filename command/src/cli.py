from typing import Callable

import click
from click_shell import shell  # type: ignore


class BvexCLI:
    def __init__(
        self, handle_set_bps: Callable[[int], None], handle_get_bps: Callable[[], int]
    ):
        self.handle_set_bps = handle_set_bps
        self.handle_get_bps = handle_get_bps
        self.app = self.create_app()

    def create_app(self):
        @shell(prompt="bvex-cli > ", intro="Bvex cli...")
        def app():
            pass

        @app.command()
        @click.argument("bits_per_second", type=int)
        def setbps(bits_per_second: int):
            self.handle_set_bps(bits_per_second)

        @app.command()
        def getbps():
            click.echo(self.handle_get_bps())

        return app

    def run(self):
        self.app()


if __name__ == "__main__":
    bps = 1

    def setbps(bps_):
        global bps
        bps = bps_

    def getbps():
        global bps
        return bps

    cli = BvexCLI(setbps, getbps)
    cli.run()
