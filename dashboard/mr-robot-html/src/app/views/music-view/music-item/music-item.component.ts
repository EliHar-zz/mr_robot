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

  public id : number;
  public song : MusicItem;
  constructor(private musicService : MusicService, private route: ActivatedRoute) { }

  ngOnInit() {
    this.song = null;
    this.route.params.subscribe((params : Params) => {
        this.id = +params["index"];
        if(!isNaN(this.id)) {
          this.song = this.musicService.getSong(this.id);
        }
      }
    );
  }

  public nextSong() {
    this.id = (this.id + 1) % this.musicService.getSongs.length;
    this.song = this.musicService.getSong(this.id);
  }

  public prevSong() {
    this.id = (this.id + this.musicService.getSongs.length - 1) % this.musicService.getSongs.length;
    this.song = this.musicService.getSong(this.id);
  }
}
