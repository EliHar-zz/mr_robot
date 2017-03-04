import { Component, OnInit } from '@angular/core';
import {ActivatedRoute, Params, Router} from "@angular/router";
import {MusicItem} from "./music-item";
import {MusicService} from "../../service/music.service";

@Component({
  selector: 'mr-music-item',
  templateUrl: './music-item.component.html',
  styleUrls: ['./music-item.component.scss']
})
export class MusicItemComponent implements OnInit {

  public song : MusicItem;
  constructor(public musicService : MusicService, private route: ActivatedRoute) { }

  ngOnInit() {
    this.song = null;
    this.route.params.subscribe((params : Params) => {
        let id = +params["index"];
        if(!isNaN(id)) {
          this.song = this.musicService.getSong(id);
        }
      }
    );
  }

  public isPlaying() : boolean{
    return this.song != null &&
      this.musicService.getStatus == MusicService.MUSIC_PLAY &&
      this.musicService.getPlayingSong.getId == this.song.getId;
  }

  public nextSong() {
    if(this.song != null) {
      let id = (this.song.getId + 1) % this.musicService.getSongs.length;
      this.song = this.musicService.getSong(id);
    }
  }

  public prevSong() {
    if(this.song != null) {
      let id = (this.song.getId + this.musicService.getSongs.length - 1) % this.musicService.getSongs.length;
      this.song = this.musicService.getSong(id);
    }
  }

  public play() {
    if(this.song != null) {
      this.musicService.play(this.song.getId);
    }
  }

  public pause() {
    if(this.song != null) {
      this.musicService.pause();
    }
  }

  public stop() {
    this.musicService.stop();
  }
}
