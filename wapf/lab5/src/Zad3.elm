module Zad3 exposing (main)

import Browser
import Html exposing (Html, div, img, text, h1, h3, a, button)
import Html.Attributes exposing (src, alt, href, style)
import Html.Events exposing (onClick)

-- Record for each photo
type alias Photo =
    { id : Int
    , url : String
    , name : String
    , source : String
    , showDetails : Bool
    }

-- Model is a list of photos
type alias Model =
    List Photo

-- Initial model with photo data
init : Model
init =
    [ { id = 1, url = "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s", name = "Słońce", source = "https://source-for-sun.com", showDetails = True }
    , { id = 2, url = "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s", name = "Ziemia", source = "https://source-for-earth.com", showDetails = True }
    , { id = 3, url = "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcTRtSo2Y6Vd0BSwHoAyDbcg29WHx-wuP8IkFw&s", name = "Księżyc", source = "https://source-for-moon.com", showDetails = True }
    ]

-- Message types
type Msg
    = ToggleDetails Int

-- Update function
update : Msg -> Model -> Model
update msg model =
    case msg of
        ToggleDetails id ->
            List.map (\photo -> if photo.id == id then { photo | showDetails = not photo.showDetails } else photo) model

-- View function
main : Program () Model Msg
main =
    Browser.sandbox { init = init, update = update, view = view }

view : Model -> Html Msg
view model =
    div []
        ( h1 [] [ text "Galeria" ]
        :: List.map viewSinglePhoto model
        )

viewSinglePhoto : Photo -> Html Msg
viewSinglePhoto photo =
    div []
        ( h3 [] [ text photo.name ]
        :: button [ onClick (ToggleDetails photo.id) ] [ text (if photo.showDetails then "Ukryj szczegóły" else "Pokaż szczegóły") ]
        :: (if photo.showDetails then
                [ img [ src photo.url, alt photo.name, style "width" "300px", style "height" "auto" ] []
                , div []
                    [ text "Source: "
                    , a [ href photo.source ] [ text photo.source ]
                    ]
                ]
            else
                [])
        )
